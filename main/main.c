#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/time.h>

#include "config.h"
#include "init.h"
#include "sock.h"
#include "i2c.h"

QueueHandle_t queue;

void blink_task() {
	await_init(); // `blink_task` uses GPIO

	while (true) {
		cyw43_arch_gpio_put(LED_PIN, 0);
		vTaskDelay(250 / portTICK_PERIOD_MS);
		cyw43_arch_gpio_put(LED_PIN, 1);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}

int main() {
	init();

	// change queue size(?) + queue
	uint8_t i2cData[2];
	queue = xQueueCreate(10, sizeof(i2cData));

	xTaskCreate((TaskFunction_t) blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate((TaskFunction_t) serve_task, "serve", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate((TaskFunction_t) bus_task, "bus", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	vTaskStartScheduler();

	while(1) {
		// we should have never gotten here
		printf("Why are we here?!\n");
	}

	return 0;
}

