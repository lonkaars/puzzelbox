#include <FreeRTOS.h>
#include <task.h>

#include "config.h"
#include "tasks.h"

#include "blink.h"
#include "i2c.h"
#include "sock.h"

void init_tasks() {
	xTaskCreate((TaskFunction_t) blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
#ifndef CFG_SRV_DISABLE
	xTaskCreate((TaskFunction_t) serve_task, "serve", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
#endif
	xTaskCreate((TaskFunction_t) bus_task, "bus", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
}
