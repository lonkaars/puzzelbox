#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv-mod
 * \defgroup pb_mod Module
 * \brief Puzzle module metadata and auxiliary utility functions
 * \{
 */

/**
 * \brief Puzzle module name
 * \note This constant is optional to define, its default value is "???"
 */
extern const char * PB_MOD_NAME;
/**
 * \brief Puzzle module bus address
 * \warning This variable **must** be defined by the user
 */
extern const i2c_addr_t PB_MOD_ADDR;

/**
 * \brief Platform-specific blocking delay function
 *
 * FIXME: this entire function should be removed (see handover: RP2040 I2C
 * limitations)
 */
void pb_mod_blocking_delay_ms(unsigned long ms);

/// \}

/**
 * \ingroup pbdrv-mod
 * \defgroup pb_i2c I2C
 * \brief I2C send/receive handlers
 *
 * If there is no existing \ref pb_drv "driver" for the microcontroller on
 * which you want to use the puzzle bus driver, you may implement the following
 * in order to use pbdrv:
 *
 * - The \c pb_i2c_recv() function must be **called** for every received I2C
 *   message
 * - The \c pb_i2c_send() function must be **implemented** using the
 *   platform/device-specific I2C write function
 *
 * \{
 */

/**
 * \brief handle a received message from the I2C bus (puzzle bus)
 *
 * This function attempts to parse an I2C message as a puzzle bus message, and
 * calls the appropriate handler for the message if it is considered valid.
 * Invalid messages are silently ignored.
 *
 * \param buf pointer to message content
 * \param sz size of \p buf
 *
 * \note This function should not be directly called from an ISR. Please use
 * FreeRTOS's \c xTimerPendFunctionCallFromISR() or a similar scheduler-based
 * deferred function call mechanism instead.
 */
void pb_i2c_recv(const uint8_t * buf, size_t sz);
/**
 * \brief send a message in master-mode on the I2C bus (puzzle bus)
 *
 * This function sends an I2C message to the address specified by \p i2c_addr.
 *
 * \param i2c_addr address of slave controller
 * \param buf pointer to message content
 * \param sz size of \p buf
 */
void pb_i2c_send(i2c_addr_t i2c_addr, const uint8_t * buf, size_t sz);

/// \}

/// \ingroup pb_hook
/// \{

/**
 * \defgroup pb_hook_mod_state State
 * \brief Provide your own global state variable
 *
 * If your puzzle module defines its own global \c pb_global_state_t, you can
 * tell the driver to use it by implementing these functions. These functions
 * are also used internally by the driver.
 *
 * \{
 */

/**
 * \brief global state read hook
 * \return current value of global state enum
 *
 * The default implementation of this function uses an internal global state
 * variable in \ref pbdrv.
 */
pb_global_state_t pb_hook_mod_state_read();
/**
 * \brief global state write hook
 * \param state new value of global state enum
 *
 * The default implementation of this function uses an internal global state
 * variable in \ref pbdrv.
 */
void pb_hook_mod_state_write(pb_global_state_t state);

/// \}
/// \}

#ifdef __cplusplus
}
#endif

