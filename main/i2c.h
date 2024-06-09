#pragma once
// https://github.com/raspberrypi/pico-examples/tree/master/i2c

#define MAX_SLAVES 10

//! looking for slave addresses and requesting updates
void bus_task();

