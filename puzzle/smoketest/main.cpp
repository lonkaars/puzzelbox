#include <Arduino.h>
#include "lib/pbdrv/pb-types.h"
#include "lib/pbdrv/pb-mod.h"

#define EXTERNAL_LED_PIN 13
#define BUTTON_PIN 10 // Define the pin where the external button is connected

pb_global_state_t state = PB_GS_NOINIT;

void setup() {
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

pb_global_state_t pb_hook_mod_state_read() {
  return state;
}

void pb_hook_mod_state_write(pb_global_state_t _state) {
  state = _state;
}

void loop() {
	digitalWrite(EXTERNAL_LED_PIN, state == PB_GS_PLAYING);

	if (!digitalRead(BUTTON_PIN)) return;
	state = PB_GS_SOLVED;
}
