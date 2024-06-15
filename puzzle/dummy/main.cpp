#include <Arduino.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pb-mod.h"

const char * PB_MOD_NAME = "dummy";
const i2c_addr_t PB_MOD_ADDR = 0x69;

void setup() {
	Serial.begin(115200);
}

void loop() {
	Serial.write("Hello world!\r\n");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

