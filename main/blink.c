#include <FreeRTOS.h>
#include <task.h>
#include <pico/cyw43_arch.h>

#include "blink.h"
#include "config.h"

void blink_task() {
	while (true) {
		cyw43_arch_gpio_put(CFG_LED_PIN, 1);
		vTaskDelay(50 / portTICK_PERIOD_MS);
		cyw43_arch_gpio_put(CFG_LED_PIN, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

