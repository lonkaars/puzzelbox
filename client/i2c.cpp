#include <stdio.h>
#include <stdlib.h>

#include "i2ctcpv1.h"
#include "sock.h"
#include "xxd.h"

#include "pb.h"
#include "pb-types.h"

// #include "pb/mod/main.h"

bool i2c_dump_send = true;
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

static void i2c_handle_cmd_read(uint16_t, const char *, size_t);

void i2c_recv(uint16_t addr, const char * data, size_t data_size) {
	if (i2c_dump_recv) {
		printf("[%s] addr(0x%02x) data(0x%02lx):\n", __FUNCTION__, addr, data_size);
		xxd(data, data_size);
	}
}

// static void i2c_handle_cmd_read(uint16_t i2c_addr, const char * buf, size_t sz) {
// 	if (sz < 2) return; // require data address + 1 byte of data
// 	pb_cmd_read_t * cmd = (pb_cmd_read_t *) buf;
// 	sz--; // sz now represents size of cmd->data
// 
// 	if (i2c_addr == BUSADDR_MAIN && cmd->address == 0x01) {
// 		if (sz % 2 != 0) return; // invalid data
// 		for (size_t offset = 0; offset < sz; offset += sizeof(pb_mod_main_mod_t)) {
// 			pb_mod_main_mod_t * mod = (pb_mod_main_mod_t *) (cmd->data + offset);
// 			printf("module at addr 0x%02x with state %d\n", mod->addr, mod->state);
// 		}
// 	}
// }

