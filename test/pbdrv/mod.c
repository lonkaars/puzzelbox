#include <stdio.h>

#include "pb-mod.h"
#include "pb-types.h"

const char * PB_MOD_NAME = "test";
const i2c_addr_t PB_MOD_ADDR = 0x08;

void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	printf("[0x%02x]: buf[%lu]\n", addr & 0x7f, sz);
}
