#pragma once

#include <stdint.h>
#include <stddef.h>

//! start listening for TCP socket requests
void serve_task();

void i2c_send(uint16_t addr, const char * data, size_t data_size);
void i2c_recv(uint16_t addr, const char * data, size_t data_size);

