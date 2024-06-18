#include <FreeRTOS.h>
#include <task.h>

#include "../../pb-types.h"

__weak void pb_mod_blocking_delay_ms(unsigned long ms) {
	vTaskDelay(ms / portTICK_PERIOD_MS);
}

