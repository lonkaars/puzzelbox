#pragma once
#include <pico/cyw43_arch.h>

// wifi credentials
#define CONF_NET_SSID "network name"
#define CONF_NET_PASS "network password"
// max duration (milliseconds) for establishing wifi connection
#define CONF_NET_CONN_TIMEOUT 10000

#include <cyw43_country.h>
#define CONF_NET_COUNTRY CYW43_COUNTRY_NETHERLANDS

#define CONF_SRV_PORT 9191

#define LED_PIN CYW43_WL_GPIO_LED_PIN

