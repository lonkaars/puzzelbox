#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>

#include "config.h"
#include "init.h"

void blink_task() {
	while (true) {
		cyw43_arch_gpio_put(LED_PIN, 0);
		vTaskDelay(250 / portTICK_PERIOD_MS);
		cyw43_arch_gpio_put(LED_PIN, 1);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}

int main() {
	init();

	xTaskCreate((TaskFunction_t) blink_task, "blink", 128, NULL, 1, NULL);
	vTaskStartScheduler();
}

