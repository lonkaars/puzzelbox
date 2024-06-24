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
	

	for (size_t i = 0; i < modules_size; i++) {
		pb_buf_t buf = pb_send_state_req();
		
		pb_buf_free(&buf);
	}
}

void update_state() {
	// TODO: Add calculation(?) to get global state
	// all states idle == idle -> set first address as playingz
	// all states solved == solved
	// any state plater == playing

	pb_global_state_t module_state;
	bool playing = false; 	// default false -> loop through modules to see if one is playing -> set to true
	bool solved = true;		// default true	 -> loop through modules to see if any is not solved -> set to false

	for (size_t i = 0; i < modules_size; i++) {
		module_state = modules[i].state;
		if (module_state != PB_GS_SOLVED)
			solved == false;	
		
		if (module_state == PB_GS_PLAYING)
			playing == true;
	}

	// set state if no further processing is needed
	if (solved == true) {
		pb_hook_mod_state_write(PB_GS_SOLVED);
		return;
	} else if (playing == true) {
		pb_hook_mod_state_write(PB_GS_PLAYING);
		return;
	}

	// IF no module playing, get/set next module THAT IS NOT SOLVED to playing
	// and set mc state to playing
	// pb_i2c_send(addr, buff.msg, buff.size)
	
	for (size_t i = 0; i < modules_size; i++) {
		module_state = modules[i].state;
		if (module_state == PB_GS_IDLE) {
			pb_buf_t buff = pb_send_state_set(PB_GS_PLAYING);
			pb_i2c_send(modules[i].sender, (uint8_t*)buff.data, buff.size);
			pb_hook_mod_state_write(PB_GS_PLAYING);
			return;
		}
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
