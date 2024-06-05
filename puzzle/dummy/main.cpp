#include <Arduino.h>
#include <Wire.h>

#include "pb/drv/arduino/mod.h"

const char * PBDRV_MOD_NAME = "dummy";
const i2c_addr_t PBDRV_MOD_ADDR = 0x20;

void setup() {
	pbdrv_setup();
}

void loop() {
	pbdrv_i2c_send(0x00, (uint8_t *) "hoi", 3);
	delay(100);
}

