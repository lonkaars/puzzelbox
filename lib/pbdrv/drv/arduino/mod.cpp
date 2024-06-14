#ifndef ARDUINO
#error This driver only works on the Arduino platform!
#endif

#include <Arduino.h>
#include <Wire.h>

#include <stdlib.h>
#include <stdint.h>

#include "../../pb.h"
#include "../../pb-mod.h"
#include "mod.h"

static void recv_event(int bytes) {
	uint8_t * data = (uint8_t *) malloc(bytes);
	size_t size = 0;
	while (Wire.available())
		data[size++] = Wire.read();

	pb_i2c_recv(data, size);
}

void pb_setup() {
	Wire.begin((int) PB_MOD_ADDR);
	Wire.setWireTimeout(PB_TIMEOUT_US, true);
	Wire.setClock(PB_CLOCK_SPEED_HZ);
	Wire.onReceive(recv_event);
}

__weak void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	Wire.beginTransmission((int) addr);
	Wire.write(buf, sz);
	Wire.endTransmission(true);
	Wire.setWireTimeout(PB_TIMEOUT_US, true);
}

