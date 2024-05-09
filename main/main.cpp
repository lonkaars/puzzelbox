#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/time.h>

#include "config.h"
#include "init.h"

#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/opt.h>

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

	for (int i = 5; i > 0; i--) {
		printf("starting in %d...\n", i);
		sleep_ms(1000);
	}

	// this should compile but not work
	lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	xTaskCreate((TaskFunction_t) blink_task, "blink", 128, NULL, 1, NULL);
	vTaskStartScheduler();
}

