#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

#include "config.h"
#include "init.h"
#include "tasks.h"
#include "drv/rp2040/mod.h"

static void init_stdio() {
	stdio_init_all();
}

static void init_cyw34() {
	if (cyw43_arch_init_with_country(CFG_NET_COUNTRY))
		panic("cyw43_arch_init_with_country failed\n");
}

static void init_wifi() {
	// enable 'station' mode (connect to an access point instead of acting like one)
	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(CFG_NET_SSID, CFG_NET_PASS, CFG_NET_AUTH, CFG_NET_CONN_TIMEOUT))
		panic("cyw43_arch_wifi_connect failed\n");

	// TODO: announce hostname(?)
}

static void init_i2c() {
	gpio_set_function(CFG_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(CFG_SCL_PIN, GPIO_FUNC_I2C);

	pbdrv_setup();
}

static void async_init() {
	init_cyw34();
	init_i2c();
#ifndef CFG_NET_DISABLE
	init_wifi();
#endif
	init_tasks();

	// delete self
	vTaskDelete(NULL);
}

void init() {
	// used for debug `printf` and `panic` on errors
	init_stdio();

	// defer other initialization until the task scheduler is running (important)
	xTaskCreate((TaskFunction_t) async_init, "init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL);
}

