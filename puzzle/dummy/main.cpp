#include <Arduino.h>

#include <FreeRTOS.h>
#include <task.h>

void setup() {
	Serial.begin(115200);
}

void loop() {
	Serial.write(".");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

