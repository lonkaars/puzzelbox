#pragma once
#include <cyw43_country.h>
#include <pico/cyw43_arch.h>

/**
 * \ingroup main
 * \defgroup main_config Config
 * \brief Configuration options
 *
 * The main controller firmware is configured statically (i.e. through
 * compile-time defined options). Because the configuration is likely to
 * contain Wi-Fi credentials, this file is not tracked under version control.
 *
 * Before compiling the main controller fimrware, a file (`config.h`) must be
 * created by the user with the following format:
 *
 * ```c
 * #pragma once
 *
 * // define non-default options here
 *
 * #include "config.def.h"
 * ```
 *
 * \note `config.def.h` contains preprocessor logic that tries to ensure a
 * correct configuration. The default configuration has the following settings:
 * - Wi-Fi is disabled (prints a warning during compilation because it was not
 *   explicitly disabled by the user)
 * - The TCP server is disabled (due to Wi-Fi being disabled)
 *
 * \note The exact default values of each configuration option, and all
 * available options are listed below.
 *
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
//! Radio communications country
#define CFG_NET_COUNTRY CYW43_COUNTRY_WORLDWIDE
#endif
#ifndef CFG_NET_COUNTRY
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
#ifndef CFG_SDA0_PIN
//! I2C 0 SDA pin
#define CFG_SDA0_PIN 16
#endif
#ifndef CFG_SCL0_PIN
//! I2C 0 SCL pin
#define CFG_SCL0_PIN 17
#endif
#ifndef CFG_SDA1_PIN
//! I2C 1 SDA pin
#define CFG_SDA1_PIN 18
#endif
#ifndef CFG_SCL1_PIN
//! I2C 1 SCL pin
#define CFG_SCL1_PIN 19
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
