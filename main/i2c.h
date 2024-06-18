#pragma once
// https://github.com/raspberrypi/pico-examples/tree/master/i2c
// https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html

#define MAX_SLAVES 10
#define MAX_TIMEOUT_TIME 50 //ms

//! looking for slave addresses and requesting updates
void bus_task();

