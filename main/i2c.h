#pragma once
// https://github.com/raspberrypi/pico-examples/tree/master/i2c

#include <stddef.h>
#include <stdint.h>
#include <hardware/i2c.h>

#define SDA_PIN 16
#define SCL_PIN 17
#define I2C_PORT i2c0
#define MAX_SLAVES 10

/**
 * \brief initialize all required gpio for i2c usage on the pico
 * 
 * This functions only initializes the standard gpio required to start i2c
 * communications.
 *
 * \note Tasks shouldn't depend on any other module in the main controller
 */
void init_i2c();

/**
 * \brief read data from addr with length len from i2c bus.
 * 
 * This functions reads data from a specific address on the i2c bus,
 * the output var will hold the data which was read from said address with
 * length len.
 */
int read_i2c(uint8_t addr, uint8_t *output, size_t len);

/**
 * \brief write data to addr with length len from i2c bus.
 * \param addr
 * \param input
 * \param len
 * This functions writes data to a specific address on the i2c bus,
 * the input var holds the data which will be written to the given 
 * address with length len.
 */
int write_i2c(uint8_t addr, uint8_t *input, size_t len);

/** \brief looking for slave addresses and requesting updates */
void bus_task();
