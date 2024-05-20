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

	void send(char* buf, size_t buf_sz);

private:
	void sock_task();
	void sock_close();

	std::thread* _thread = nullptr;

	char* _addr = NULL;
	uint16_t _port = 0;

	int _fd = -1;

};

extern PBSocket* sock;

void i2c_send(uint16_t addr, char* data, size_t data_size);
void i2c_recv(uint16_t addr, char* data, size_t data_size);

