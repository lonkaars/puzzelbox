#include <Arduino.h>
#include <Wire.h>

#include <FreeRTOS.h>
#include <task.h>

#include "drv/arduino/mod.h"
#include "pb-mod.h"

const char * PB_MOD_NAME = "dummy";
const i2c_addr_t PB_MOD_ADDR = 0x69;

void testTask() {
	while(1) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		Serial.write("Hello world!\r\n");
	}
}

void setup() {
	Serial.begin(115200);
	Serial.write("setup called\r\n");
	xTaskCreate((TaskFunction_t) testTask, "test", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	Serial.write("setup done\r\n");
}

