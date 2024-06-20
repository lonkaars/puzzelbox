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

static pb_global_state_t _global_state = PB_GS_IDLE; 
pb_puzzle_module_t modules[CFG_PB_MOD_MAX];
size_t modules_size = 0;

static void state_exchange() {
	pb_global_state_t new_state;
	// TODO: Add calculation(?) to get global state
	// noinit -> not possible, idle -> all idle, solved -> all solved, playing -> one playing
	for (size_t i = 0; i < modules_size; i++) {
		
	}
	pb_hook_mod_state_write(new_state);

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
	pb_puzzle_module_t tmp_module = {msg->sender, PB_GS_NOINIT};
	modules[modules_size++] = tmp_module;
	printf("i2c: registered puzzle module w/ address 0x%02x\n", msg->sender);
}

void pb_route_cmd_state_res(pb_msg_t * msg) {
	pb_cmd_state_t * cmd = msg->cmd;
	i2c_addr_t sender = msg->sender;
	
	// update sender state
	for( size_t i = 0; i < modules_size; i++ ) {
		if (modules[i].sender == sender) {
			modules[i].state = (pb_global_state_t)cmd;
			break;
		}
	}
}

pb_global_state_t pb_hook_mod_state_read() {
	return _global_state;
}

void pb_hook_mod_state_write(pb_global_state_t state) {
	_global_state = state;
}
