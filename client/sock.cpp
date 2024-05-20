#include <unistd.h>
#include <cstdio>

#include <thread>

#include "sock.h"
#include "rl.h"

PBSocket::PBSocket() {
	printf("Init PBSocket!\n");
}

PBSocket::PBSocket(char* addr, uint16_t port) : PBSocket() {
	connect(addr, port);
}

void PBSocket::connect(char* addr, uint16_t port) {
	printf("Connect to %s on port %d\n", addr, port);

	this->_thread = std::thread(&PBSocket::sock_task, this);
}

void PBSocket::sock_task() {
	while(1) {
		sleep(3);
		rl_printf("Testing asynchronous messages in prompt...\n");
	}
}

