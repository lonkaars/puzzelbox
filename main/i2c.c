#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "i2c.h"
#include "pb-mod.h"
#include "pbdrv.h"
#include "config.h"
#include "pb-buf.h"
#include "pb-send.h"

i2c_addr_t modules[CFG_PB_MOD_MAX];
size_t modules_size = 0;

static void state_exchange() {

	// TODO: Add calculation(?) to get global state

	for (size_t i = 0; i < modules_size; i++) {
		pb_buf_t buf = pb_send_state_req();
		
		pb_buf_free(&buf);
	}
}

void bus_task() {
	// do a scan of the bus
	bus_scan();

	// FIXME: this should be removed (see handover: RP2040 I2C limitations)
	// wait for 5 seconds until all handshake responses are received
	pb_mod_blocking_delay_ms(5e3);

	while(1) {
		// send my state to all puzzle modules
		state_exchange();

		// wait 1 second
		pb_mod_blocking_delay_ms(1e3);
	}
}

void pb_route_cmd_magic_res(pb_msg_t * msg) {
	if (modules_size == CFG_PB_MOD_MAX) return;
	modules[modules_size++] = msg->sender;
	printf("i2c: registered puzzle module w/ address 0x%02x\n", msg->sender);
}

void pb_route_cmd_state_res(pb_msg_t * msg) {
	pb_cmd_state_t * cmd = msg->cmd;
	// return early if state has wrong size
	if (cmd->_magic_size != sizeof(pb_cmd_state_t))
		return;

	// TODO: Get msg sender & update state in array struct (?)
}
