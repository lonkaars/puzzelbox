#include "config.h"
#include "init.h"

#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

static void init_stdio() {
	stdio_init_all();
}

static void init_cyw34() {
	if (cyw43_arch_init_with_country(CONF_NET_COUNTRY))
		panic("cyw43_arch_init_with_country failed\n");
}

static void init_wifi() {
	// enable 'station' mode (connect to an access point instead of acting like one)
	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(CONF_NET_SSID, CONF_NET_PASS, CYW43_AUTH_WPA2_AES_PSK, CONF_NET_CONN_TIMEOUT))
		panic("cyw43_arch_wifi_connect failed\n");

	// TODO: announce hostname
}

void init() {
	init_stdio();
	init_cyw34();
	init_wifi();
	// TODO: initialize i2c
}

