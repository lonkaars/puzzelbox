#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "i2c.h"
#include "lib/pbdrv/mod/main/mod.h"
#include "pb-mod.h"
#include "config.h"
#include "pb-buf.h"
#include "pb-send.h"

typedef struct {
	i2c_addr_t sender; //!< I2C address of sender
	pb_global_state_t state; //!< global state
} puzzle_module_t;

static pb_global_state_t _global_state = PB_GS_IDLE;
puzzle_module_t modules[CFG_PB_MOD_MAX] = {};
size_t modules_size = 0;
// stolen from lib/pico-sdk/src/rp2_common/hardware_i2c/i2c.c
#define i2c_reserved_addr(addr) (((addr) & 0x78) == 0 || ((addr) & 0x78) == 0x78)

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

static void update_state() {
	int idle = 0, playing = 0, solved = 0;

	// count # of modules in each state
	for (size_t i = 0; i < modules_size; i++) {
		pb_global_state_t state = modules[i].state;
		if (state == PB_GS_IDLE) idle++;
		else if (state == PB_GS_PLAYING) playing++;
		else if (state == PB_GS_SOLVED) solved++;
	}

	if (idle == modules_size) { // if all modules are in PB_GS_IDLE
		pb_hook_mod_state_write(PB_GS_IDLE);
	} else if (solved == modules_size) { // if all modules are in PB_GS_SOLVED
		pb_hook_mod_state_write(PB_GS_SOLVED);
	} else {
		pb_hook_mod_state_write(PB_GS_PLAYING);
	}

	// if a module is still playing, don't promote a next one to playing module
	if (playing > 0) return;

	for (size_t i = 0; i < modules_size; i++) {
		// find first module that is idle
		pb_global_state_t	module_state = modules[i].state;
		if (module_state != PB_GS_IDLE) continue;

		pb_buf_t buff = pb_send_state_set(PB_GS_PLAYING);
		pb_i2c_send(modules[i].sender, (uint8_t*)buff.data, buff.size);
		pb_buf_free(&buff);
	}
}

static void state_exchange() {
	update_state();
	pb_buf_t buf = pb_send_state_req();
	for (size_t i = 0; i < modules_size; i++)
		pb_i2c_send(modules[i].sender, (uint8_t *) buf.data, buf.size);
	pb_buf_free(&buf);
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
	modules[modules_size++] = (puzzle_module_t) {
		.sender = msg->sender,
		.state = PB_GS_NOINIT,
	};
	printf("i2c: registered puzzle module w/ address 0x%02x\n", msg->sender);
}

void pb_route_cmd_state_res(pb_msg_t * msg) {
	pb_cmd_state_t * cmd = msg->cmd;
	i2c_addr_t sender = msg->sender;

	for (size_t i = 0; i < modules_size; i++) {
		if (modules[i].sender != sender) continue;
		modules[i].state = cmd->state;
	}
}

pb_global_state_t pb_hook_mod_state_read() {
	return _global_state;
}

void pb_hook_mod_state_write(pb_global_state_t state) {
	_global_state = state;
}

void pb_route_cmd_prop_req(pb_msg_t * msg) {
	// send modules using buf
	// pb_cmd_prop_t cmd = msg->cmd;

	// pb_buf_t buf;

	// if(cmd == "PROPID_AJHDKADLHL") {
	// 	pb_send_reply(msg,buf);
	// }

	// pb_buf_free(buf);
}

void pb_route_cmd_prop_set(pb_msg_t * msg) {
	// if incorrect property id -> stop
	pb_cmd_prop_t * cmd = msg->cmd;
	if(cmd->propid != PB_MOD_MAIN_PROP_MODS) return;

	size_t modules_size = 0;
	puzzle_module_t modules[CFG_PB_MOD_MAX] = {};

	bus_scan();
}

