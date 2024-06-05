#include <Arduino.h>
#include <Wire.h>

void receiveEvent(int howMany){
	while (Wire.available()){
		char c = Wire.read();
		Serial.print(c, HEX);
	}
	Serial.println();
}

void setup() {
	Wire.begin(0x00);
	Wire.onReceive(receiveEvent);
}

void loop() {
	const uint8_t data[] = {0xff, 0x00, 0xde, 0xad, 0xbe, 0xef};
	Wire.beginTransmission(0x69);
	Wire.write(data, sizeof(data));
	Wire.endTransmission();
}

