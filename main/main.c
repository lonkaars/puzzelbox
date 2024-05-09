#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/time.h>

#include "config.h"
#include "init.h"

// #include <lwip/sockets.h>
// #include <lwip/sys.h>
// #include <lwip/opt.h>

void blink_task() {
	await_init(); // `blink_task` uses GPIO

	while (true) {
		cyw43_arch_gpio_put(LED_PIN, 0);
		vTaskDelay(250 / portTICK_PERIOD_MS);
		cyw43_arch_gpio_put(LED_PIN, 1);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}

void test_task() {
	int i = 0;
	while (true) {
		// stdio is initialized synchronously, so no `await_init` is needed
		printf("hello #%d...\n", ++i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

int main() {
	init();

	xTaskCreate((TaskFunction_t) blink_task, "blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate((TaskFunction_t) test_task, "test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

	vTaskStartScheduler();
}

