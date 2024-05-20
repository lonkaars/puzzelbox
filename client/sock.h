#pragma once

#include <cstdint>
#include <thread>

class PBSocket {
public:
	PBSocket();
	PBSocket(char* addr, uint16_t port);
	virtual ~PBSocket();

	void set_server(char* addr, uint16_t port);

	void sock_connect();

private:
	void sock_task();
	void sock_close();

	std::thread* _thread = nullptr;

	char* _addr = NULL;
	uint16_t _port = 0;

	int _fd = -1;

};

