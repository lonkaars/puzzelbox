#include "pb.h"
#include "pb-mod.h"

const char * PB_MOD_NAME = "client";
const i2c_addr_t PB_MOD_ADDR = PB_ADDR_MOD_MAIN;

void pb_i2c_recv(const uint8_t * buf, size_t sz) {}
void pb_i2c_send(i2c_addr_t i2c_addr, const uint8_t * buf, size_t sz) {}

