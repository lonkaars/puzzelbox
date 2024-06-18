#include <Arduino.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pb-mod.h"
#include "pb-route.h"
#include "pb-send.h"

const char * PB_MOD_NAME = "dummy";
const i2c_addr_t PB_MOD_ADDR = 0x69;

void setup() {
	Serial.begin(115200);

	pb_buf_t buf = pb_send_magic_res();
	Serial.print("response bytes:");
	for (size_t i = 0; i < buf.size; i++) {
		Serial.print(" ");
		Serial.print(buf.data[i]);
	}
	Serial.print("\r\n");
	pb_buf_free(&buf);
}

void loop() {
	Serial.write(".");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void vApplicationMallocFailedHook(void) {
	Serial.println("malloc failed!");
}

