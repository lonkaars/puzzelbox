#pragma once

#include <stddef.h>
#include <stdint.h>

#include "i2ctcpv1.h"

/**
 * \ingroup pbc
 * \defgroup pbc_i2c I2C
 * \brief I2C abstraction functions
 * \{
 */

/**
 * \brief Fake I2C send function
 *
 * This function sends an I2C message to the main controller over TCP using
 * \ref i2ctcp.
 *
 * \param addr I2C address
 * \param data Data to send
 * \param data_size size of \p data
 */
void i2c_send(uint16_t addr, const char * data, size_t data_size);
/**
 * \brief Handle received (main -> client) I2C messages
 *
 * This function is called for I2C messages both sent and received by the main
 * controller. This function tries to distinguish between sent/received
 * messages by parsing the message data as a puzzle bus message and checking if
 * the msg->sender field is equal to the main controller bus address.
 *
 * \param msg Transferred message
 */
void i2c_dump(const i2ctcp_msg_t * msg);

/// \}
