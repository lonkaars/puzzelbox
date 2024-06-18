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

i2c_addr_t modules[CFG_PB_MOD_MAX];
size_t modules_size = 0;

void bus_task() {
	bus_scan();
	vTaskDelete(NULL);
}

void pb_route_cmd_magic_res(pb_msg_t * msg) {
	if (modules_size == CFG_PB_MOD_MAX) return;
	modules[modules_size++] = msg->sender;
	printf("i2c: registered puzzle module w/ address 0x%02x\n", msg->sender);
}

