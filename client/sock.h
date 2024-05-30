#pragma once

#include <cstdint>
#include <thread>

class PBSocket {
public:
	PBSocket();
	PBSocket(const char * addr, uint16_t port);
	virtual ~PBSocket();

	void set_server(const char * addr, uint16_t port);

	void sock_connect();

	void send(const char * buf, size_t buf_sz);

private:
	void sock_task();
	void sock_close();

	std::thread* _thread = nullptr;

	const char * _addr = NULL;
	uint16_t _port = 0;

	int _fd = -1;
};

extern PBSocket* sock;

