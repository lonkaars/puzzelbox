#include <stdio.h>
#include <stdlib.h>

#include "i2ctcpv1.h"
#include "sock.h"
#include "xxd.h"

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
		printf("[%s] addr(0x%02x) data(0x%02lx):\n", __FUNCTION__, addr, data_size);
		xxd(data, data_size);
	}

	free(packed);
}

void i2c_recv(uint16_t addr, const char * data, size_t data_size) {
	if (i2c_dump_recv) {
		printf("[%s] addr(0x%02x) data(0x%02lx):\n", __FUNCTION__, addr, data_size);
		xxd(data, data_size);
	}
}

