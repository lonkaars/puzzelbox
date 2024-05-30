#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/time.h>

#include "config.h"
#include "init.h"
#include "sock.h"
#include "i2c.h"

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

	xTaskCreate((TaskFunction_t) blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate((TaskFunction_t) serve_task, "serve", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate((TaskFunction_t) bus_task, "bus", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	vTaskStartScheduler();
}

