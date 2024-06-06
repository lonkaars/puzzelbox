#include <Arduino.h>
#include <Wire.h>

#include "drv/arduino/mod.h"

// #define THOMAS

#ifdef THOMAS
#define ADDR_RX 0x69
#define ADDR_TX 0x20
#define MSG "hoi"
#define MSG_DELAY 10
#else
#define ADDR_TX 0x69
#define ADDR_RX 0x20
#define MSG "dag"
#define MSG_DELAY 9
#endif

const char * PBDRV_MOD_NAME = "dummy";
const i2c_addr_t PBDRV_MOD_ADDR = ADDR_RX;

void setup() {
	pbdrv_setup();
	Serial.begin(115200);
	pinMode(4, OUTPUT);
	digitalWrite(4, LOW);
}

void loop() {
	pbdrv_i2c_send(ADDR_TX, (uint8_t *) MSG, 4);
	delay(MSG_DELAY);
}

void pbdrv_i2c_recv(const uint8_t * data, size_t size) {
	Serial.println((char *) data);
}

