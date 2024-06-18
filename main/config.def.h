#pragma once
#include <pico/cyw43_arch.h>
#include <cyw43_country.h>

/**
 * \name Network (Wi-Fi) configuration
 * \{
 */
#ifndef CFG_NET_SSID
//! network name (SSID)
#define CFG_NET_SSID ""
#ifndef CFG_NET_DISABLE
//! disable network communication
#define CFG_NET_DISABLE
#warning No SSID defined! Disabling network communication!
#endif
#endif

#ifndef CFG_NET_PASS
//! network password
#define CFG_NET_PASS ""
#endif

#ifndef CFG_NET_AUTH
//! network security type
#define CFG_NET_AUTH CYW43_AUTH_OPEN
#endif

#ifndef CFG_NET_CONN_TIMEOUT
//! max duration (milliseconds) for establishing wifi connection
#define CFG_NET_CONN_TIMEOUT 10e3
#endif

#ifdef CFG_NET_DISABLE
#undef CFG_NET_COUNTRY
#define CFG_NET_COUNTRY CYW43_COUNTRY_WORLDWIDE
#endif
#ifndef CFG_NET_COUNTRY
//! radio communications country
#define CFG_NET_COUNTRY CYW43_COUNTRY_NETHERLANDS
#endif
/** \} */

/**
 * \name i2ctcp server configuration
 * \{
 */
#ifndef CFG_SRV_PORT
//! i2ctcp server port
#define CFG_SRV_PORT 9191
#endif

#ifdef CFG_NET_DISABLE
//! disable the i2ctcp server
#define CFG_SRV_DISABLE
#endif
/** \} */

/**
 * \name I2C configuration
 * \{
 */
#ifndef CFG_SDA_PIN
//! I^2^C SDA pin
#define CFG_SDA_PIN 16
#endif
#ifndef CFG_SCL_PIN
//! I^2^C SCL pin
#define CFG_SCL_PIN 17
#endif
/** \} */

#ifndef CFG_LED_PIN
//! status LED pin
#define CFG_LED_PIN CYW43_WL_GPIO_LED_PIN
#endif

#ifndef CFG_PB_MOD_MAX
//! maximum number of simultaniously connected puzzle modules
#define CFG_PB_MOD_MAX 8
#endif

