#include <FreeRTOS.h>
#include <task.h>

#include "pb-mod.h"
#include "pb.h"

const char * PB_MOD_NAME = "main controller";
const i2c_addr_t PB_MOD_ADDR = PB_ADDR_MOD_MAIN;
