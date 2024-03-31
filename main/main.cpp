#include <stdio.h>

#include "config.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

const unsigned int LED_PIN = CYW43_WL_GPIO_LED_PIN;

int main() {
	stdio_init_all();
	sleep_ms(2000);

	if (cyw43_arch_init_with_country(CONF_NET_COUNTRY)) {
		printf("failed to initialize\n");
		return 1;
	}
	cyw43_arch_gpio_put(LED_PIN, 1);
	printf("initialised\n");

	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(CONF_NET_SSID, CONF_NET_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
		printf("failed to connect\n");
		return 1;
	}
	printf("connected\n");

	while (true) {
		cyw43_arch_gpio_put(LED_PIN, 0);
		sleep_ms(250);
		cyw43_arch_gpio_put(LED_PIN, 1);
		sleep_ms(250);
	}
}

