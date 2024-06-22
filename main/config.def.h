#pragma once
#include <pico/cyw43_arch.h>
#include <cyw43_country.h>

/**
 * \ingroup main
 * \defgroup main_config config
 * \brief Configuration options
 * \{
 */

/**
 * \name Network (Wi-Fi) configuration
 * \{
 */
#ifndef CFG_NET_SSID
/**
 * \brief Network name (SSID)
 * \note Not defining \c CFG_NET_SSID will implicitly enable \c CFG_NET_DISABLE
 */
#define CFG_NET_SSID ""
#ifndef CFG_NET_DISABLE
/**
 * \brief Disable network communication completely
 * \note Enabling this option will implicitly enable \c CFG_SRV_DISABLE
 */
#define CFG_NET_DISABLE
#warning No SSID defined! Disabling network communication!
#endif
#endif

#ifndef CFG_NET_PASS
//! Network password
#define CFG_NET_PASS ""
#endif

#ifndef CFG_NET_AUTH
//! Network security type
#define CFG_NET_AUTH CYW43_AUTH_OPEN
#endif

#ifndef CFG_NET_CONN_TIMEOUT
//! Max duration (milliseconds) for establishing Wi-Fi connection
#define CFG_NET_CONN_TIMEOUT 10e3
#endif

#ifdef CFG_NET_DISABLE
#undef CFG_NET_COUNTRY
#define CFG_NET_COUNTRY CYW43_COUNTRY_WORLDWIDE
#endif
#ifndef CFG_NET_COUNTRY
//! Radio communications country
#define CFG_NET_COUNTRY CYW43_COUNTRY_NETHERLANDS
#endif
/// \}

/**
 * \name TCP server configuration
 * \{
 */
#ifndef CFG_SRV_PORT
//! TCP server port
#define CFG_SRV_PORT 9191
#endif
#ifdef CFG_NET_DISABLE
//! Disable the TCP server
#define CFG_SRV_DISABLE
#endif
/// \}

/**
 * \name I2C configuration
 * \{
 */
#ifndef CFG_SDA_PIN
//! I2C SDA pin
#define CFG_SDA_PIN 16
#endif
#ifndef CFG_SCL_PIN
//! I2C SCL pin
#define CFG_SCL_PIN 17
#endif
/// \}

/**
 * \name Auxiliary options
 * \{
 */
#ifndef CFG_LED_PIN
//! Status LED pin
#define CFG_LED_PIN CYW43_WL_GPIO_LED_PIN
#endif
#ifndef CFG_PB_MOD_MAX
//! Maximum number of simultaniously connected puzzle modules
#define CFG_PB_MOD_MAX 8
#endif
/// \}

/// \}

