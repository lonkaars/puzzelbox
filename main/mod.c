#include <FreeRTOS.h>
#include <task.h>

#include "pb.h"
#include "pb-mod.h"

const char * PB_MOD_NAME = "main controller";
const i2c_addr_t PB_MOD_ADDR = PB_ADDR_MOD_MAIN;

void pb_mod_blocking_delay_ms(unsigned long ms) {
	vTaskDelay(ms / portTICK_PERIOD_MS);
}

