#ifndef ARDUINO
#error This driver only works on the Arduino platform!
#endif

#include <Arduino.h>
#include <Wire.h>

#include <stdlib.h>
#include <stdint.h>

#include "../../pb.h"
#include "mod.h"

static void recv_event(int bytes) {
	uint8_t * data = (uint8_t *) malloc(bytes);
	size_t size = 0;
	while (Wire.available()) {
		data[size++] = Wire.read();
	}

	pbdrv_i2c_recv(data, size);
}

void pbdrv_setup() {
	Wire.begin((int) PBDRV_MOD_ADDR);
	Wire.setWireTimeout(PB_TIMEOUT_US, true);
	Wire.setClock(PB_CLOCK_SPEED_HZ);
	Wire.onReceive(recv_event);
}

__weak void pbdrv_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	uint8_t error;
	uint8_t retry = 4;
	do {
		error = 0;
		Wire.beginTransmission((int) addr);
		size_t written = Wire.write(buf, sz);
		if (written != sz)
			error++;
		error += Wire.endTransmission(true);
		Wire.setWireTimeout(PB_TIMEOUT_US, true);

		if(retry == 0) break;
		retry--;
	} while (error);

	if (error)
		Serial.print(error, DEC);
}

