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

#include "i2ctcpv1.h"
#include "sock.h"
#include "rl.h"
#include "xxd.h"

using std::logic_error;
using std::thread;

PBSocket::PBSocket() { }
PBSocket::PBSocket(const char * addr, uint16_t port) : PBSocket() {
	set_server(addr, port);
}

PBSocket::~PBSocket() {
	// stop TCP listen thread
	if (_thread != nullptr) {
		_thread->detach();
		delete _thread;
	}

	sock_close();
}

void PBSocket::set_server(const char * addr, uint16_t port) {
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

void PBSocket::send(const char * buf, size_t buf_sz) {
	write(_fd, buf, buf_sz);
}

void PBSocket::sock_task() {
	i2ctcp_msg_t input;

	while(1) {
		char buf[80];
		ssize_t bytes = read(_fd, buf, sizeof(buf));

		if (bytes == -1) {
			rl_printf("error: %s (%d)\n", strerror(errno), errno);
			break;
		}

		// skip empty frames
		if (bytes == 0) continue;

		int ret = i2ctcp_read(&input, buf, bytes);

		// header read error
		if (ret < 0) {
			rl_printf("i2ctcp_read error!\n");
			break;
		}

		// continue reading if more bytes needed...
		if (ret > 0) continue;

		// message read completely!
		i2c_recv(input.addr, input.data, input.length);
		free(input.data);
	}

	sock_close();
}

bool i2c_dump_send = false;
bool i2c_dump_recv = true;

void i2c_send(uint16_t addr, const char * data, size_t data_size) {
	i2ctcp_msg_t msg = {
		.addr = addr,
		.data = (char *) data,
		.length = data_size,
	};

	char* packed;
	size_t size;
	if (!i2ctcp_write(&msg, &packed, &size)) return;

	sock->send(packed, size);
	if (i2c_dump_send) {
		printf("[i2c send] data(0x%02lx):\n", data_size);
		xxd(data, data_size);
	}
}

void i2c_recv(uint16_t addr, const char * data, size_t data_size) {
	if (i2c_dump_recv) {
		printf("[i2c recv] data(0x%02lx):\n", data_size);
		xxd(data, data_size);
	}
}

