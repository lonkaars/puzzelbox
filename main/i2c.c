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

static void bus_scan() {
	pb_buf_t buf = pb_send_magic_req();

	// check for all 7-bit addresses
	uint16_t addr_max = 1 << 7;
	for (uint16_t addr = 0x00; addr < addr_max; addr++) {
		pb_i2c_send(addr, (uint8_t *) buf.data, buf.size);
	}

	pb_buf_free(&buf);
}

static void state_exchange() {
	for (size_t i = 0; i < modules_size; i++) {
		pb_buf_t buf = pb_send_state_req();
		
		pb_buf_free(&buf);
	}
}

void bus_task() {
	// do a scan of the bus
	bus_scan();

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

