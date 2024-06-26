#include <stdio.h>
#include <stdlib.h>

#include "i2ctcpv1.h"
#include "rl.h"
#include "sock.h"
#include "xxd.h"

#include "pb-buf.h"
#include "pb-mod.h"
#include "pb-msg.h"
#include "pb-types.h"
#include "pb.h"

#ifdef DEBUG
bool i2c_dump_send = true;
bool i2c_dump_recv = true;
#else
bool i2c_dump_send = false;
bool i2c_dump_recv = false;
#endif

void i2c_send(uint16_t addr, const char * data, size_t data_size) {
	i2ctcp_msg_t msg = {
		.addr = addr,
		.data = (char *) data,
		.length = data_size,
	};

	char * packed;
	size_t size;
	if (!i2ctcp_write(&msg, &packed, &size)) return;

	sock->send(packed, size);

	free(packed);
}

void i2c_dump(const i2ctcp_msg_t * msg) {
	pb_buf_t buf = {
		.data = (char *) msg->data,
		.size = msg->length,
	};
	pb_msg_t * pb_msg = pb_msg_read(&buf);

	// ignore invalid messages
	if (msg == NULL) return;
	// NOTE: I feel like this check is OK to do because the main controller isn't
	// supposed to be written to by I2C controllers that aren't puzzle modules.
	// I2C doesn't tell the receiver *who's* currently addressing it, which means
	// we have to assume all messages written to the main controller are puzzle
	// bus messages, and use the puzzle bus message to get the sender's address.
	i2c_addr_t sender, receiver;
	sender = receiver = pb_msg->sender;

	bool send = sender == PB_MOD_ADDR; // = PB_ADDR_MOD_MAIN

	if (send) receiver = msg->addr;
	else receiver = PB_MOD_ADDR;

	pb_msg_free(pb_msg);

	if (send && !i2c_dump_send) return;
	if (!send && !i2c_dump_recv) return;

	_rl_printf_start();

	const char * direction = send ? "send" : "recv";
	printf("[I2C %s] 0x%02x -> 0x%02x data(0x%02lx):\n", direction, sender,
		   receiver, msg->length);
	xxd(msg->data, msg->length);

	_rl_printf_stop();
}
