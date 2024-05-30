#include "config.h"
#include "init.h"
#include "i2c.h"

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

EventGroupHandle_t init_complete;

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

	/* WERKT GEWOON NIET MET DEZE LIJNEN CODE */
	// if (cyw43_arch_wifi_connect_timeout_ms(CONF_NET_SSID, CONF_NET_PASS, CONF_NET_AUTH, CONF_NET_CONN_TIMEOUT))
	// 	panic("cyw43_arch_wifi_connect failed\n");
	/* WERKT GEWOON NIET MET DEZE LIJNEN CODE */

	printf("connected to Wi-Fi\n");

	// TODO: announce hostname(?)
}

static void async_init() {
	init_cyw34();
	init_i2c();
	init_wifi();

	xEventGroupSetBits(init_complete, 1);

	// delete self
	vTaskDelete(NULL);
}

void init() {
	init_complete = xEventGroupCreate();

	// used for debug `printf` and `panic` on errors
	init_stdio();

	// defer other initialization until the task scheduler is running (important)
	xTaskCreate((TaskFunction_t) async_init, "init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);
}

void await_init() {
	xEventGroupWaitBits(init_complete, 1, pdFALSE, pdFALSE, portMAX_DELAY);
}

