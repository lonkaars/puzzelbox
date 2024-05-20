#pragma once

#include <cstdint>
#include <thread>

class PBSocket {
public:
	PBSocket(char* addr, uint16_t port);

	void connect(char* addr, uint16_t port);

private:
	PBSocket();

	void sock_task();

	std::thread _thread;

};


