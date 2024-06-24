#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "i2c.h"
#include "pb-mod.h"
#include "config.h"
#include "pb-buf.h"
#include "pb-send.h"

// stolen from lib/pico-sdk/src/rp2_common/hardware_i2c/i2c.c
#define i2c_reserved_addr(addr) (((addr) & 0x78) == 0 || ((addr) & 0x78) == 0x78)

i2c_addr_t modules[CFG_PB_MOD_MAX];
size_t modules_size = 0;

static void bus_scan() {
	pb_buf_t buf = pb_send_magic_req();

	// check for all 7-bit addresses
	uint16_t addr_max = 1 << 7;
	for (uint16_t addr = 0x00; addr < addr_max; addr++) {
		if (i2c_reserved_addr(addr)) continue;
		if (addr == PB_MOD_ADDR) continue;

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
		vTaskDelay(1e3 / portTICK_PERIOD_MS);
	}
}

/**
 * \ingroup main_pb_override
 * \anchor main_route_cmd_magic_res
 *
 * This function registers the I2C address of the puzzle module that replied to
 * the \c MAGIC \c REQ command into a list of "known puzzle modules", which are
 * then periodically updated during gameplay.
 *
 * \note Up to \ref CFG_PB_MOD_MAX puzzle modules can be registered
 * simultaniously.
 */
void pb_route_cmd_magic_res(pb_msg_t * msg) {
	if (modules_size == CFG_PB_MOD_MAX) return;
	modules[modules_size++] = msg->sender;
	printf("i2c: registered puzzle module w/ address 0x%02x\n", msg->sender);
}

