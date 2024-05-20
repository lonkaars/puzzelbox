#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include <thread>

#include "sock.h"
#include "rl.h"

using std::logic_error;
using std::thread;

PBSocket::PBSocket() { }
PBSocket::PBSocket(char* addr, uint16_t port) : PBSocket() {
	set_server(addr, port);
	sock_connect();
}

PBSocket::~PBSocket() {
	// stop TCP listen thread
	if (_thread != nullptr) {
		_thread->detach();
		delete _thread;
	}

	sock_close();
}

void PBSocket::set_server(char* addr, uint16_t port) {
	_addr = addr;
	_port = port;
}

void PBSocket::sock_connect() {
	if (_addr == NULL) throw logic_error("no server address defined");
	if (_port == 0) throw logic_error("no server port defined");

	if (_thread != nullptr) throw logic_error("already connected");

	rl_printf("connecting to %s on port %d...\n", _addr, _port);

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) throw logic_error("socket create failed");
	
	struct sockaddr_in server = {
		.sin_family = AF_INET,
		.sin_port = htons(_port),
		.sin_addr = {
			.s_addr = inet_addr(_addr),
		},
	};
	int ret = connect(_fd, (struct sockaddr*) &server, sizeof(server));
	if (ret != 0) throw logic_error(strerror(errno));

	this->_thread = new thread(&PBSocket::sock_task, this);
}

void PBSocket::sock_close() {
	if (_fd < 0) return; // already closed
	close(_fd);
	_fd = -1;
}

void PBSocket::sock_task() {
	while(1) {
		char buf[80];
		ssize_t bytes = read(_fd, buf, sizeof(buf));

		if (bytes == -1) {
			rl_printf("error: %s (%d)\n", strerror(errno), errno);
			sock_close();
			break;
		}

		if (bytes > 0) {
			rl_printf("received %d bytes\n", bytes);
		}
	}
}

