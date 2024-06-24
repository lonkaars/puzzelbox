#pragma once

#include <stdint.h>
#include <stddef.h>

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
 * \brief Fake I2C receive handler
 *
 * This function is called for I2C messages received by the main controller and
 * forwarded to \ref pbc.
 *
 * \param data Received data
 * \param data_size size of \p data
 */
void i2c_recv(const char * data, size_t data_size);

/// \}
