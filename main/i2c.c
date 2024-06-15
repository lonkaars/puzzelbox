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

void bus_task() {
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	
	bus_scan();
	vTaskDelete(NULL);
}

void pb_route_cmd_magic_res(pb_msg_t * msg) {
	printf("got a magic response from 0x%02x!\n", msg->sender);
}

