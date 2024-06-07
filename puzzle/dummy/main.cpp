#include <Arduino.h>
#include <Wire.h>

#include "drv/arduino/mod.h"

#ifdef TEST_A
#define ADDR_RX 0x69
#define ADDR_TX 0x20
#define MSG "aa"
#define MSG_SIZE 3
#define MSG_DELAY 10
#endif

#ifdef TEST_B
#define ADDR_TX 0x69
#define ADDR_RX 0x20
#define MSG "bbbbbbbb"
#define MSG_SIZE 9
#define MSG_DELAY 10
#endif

const char * PBDRV_MOD_NAME = "dummy";
const i2c_addr_t PBDRV_MOD_ADDR = ADDR_RX;

void setup() {
	pbdrv_setup();
	Serial.begin(115200);
}

void loop() {
	pbdrv_i2c_send(ADDR_TX, (uint8_t *) MSG, MSG_SIZE);
	delay(MSG_DELAY);
}

void pbdrv_i2c_recv(const uint8_t * data, size_t size) { }

