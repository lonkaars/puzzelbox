#pragma once
// https://github.com/raspberrypi/pico-examples/tree/master/i2c

#include <stddef.h>
#include <stdint.h>
#include <hardware/i2c.h>

#define MAX_SLAVES 10

/** \brief looking for slave addresses and requesting updates */
void bus_task();

