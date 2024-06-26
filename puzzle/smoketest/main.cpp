#include <Arduino.h>
#include "lib/pbdrv/pb-types.h"
#include "lib/pbdrv/pb-mod.h"

//! LED pin (on-board LED on Arduino Uno board) \ingroup puz_smoketest
#define EXTERNAL_LED_PIN 13
//! External button pin (button shorts this pin to ground) \ingroup puz_smoketest
#define BUTTON_PIN 10

pb_global_state_t state = PB_GS_NOINIT;

pb_global_state_t pb_hook_mod_state_read() {
  return state;
}

void pb_hook_mod_state_write(pb_global_state_t _state) {
  state = _state;
}

void setup() {
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
	digitalWrite(EXTERNAL_LED_PIN, state == PB_GS_PLAYING);

	if (!digitalRead(BUTTON_PIN)) // button is inverted
		state = PB_GS_SOLVED;
}

