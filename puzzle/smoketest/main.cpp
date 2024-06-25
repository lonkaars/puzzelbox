#include <Arduino.h>
#include "lib/pbdrv/pb-types.h"
#include "lib/pbdrv/pb-mod.h"

#define EXTERNAL_LED_PIN 13
#define BUTTON_PIN 10  // Define the pin where the external button is connected

// Puzzle state
pb_global_state_t puzzleState = PB_GS_NOINIT;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonState = HIGH;          // the previous reading from the input pin
int buttonState = HIGH;              // the current state of the button

void setup() {
  Serial.begin(115200);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(EXTERNAL_LED_PIN, LOW);  // Ensure the external LED is off initially

  Serial.println("System initialized.");
}

void led_blink(int time){
  digitalWrite(EXTERNAL_LED_PIN, HIGH);
  delay(time);
  digitalWrite(EXTERNAL_LED_PIN, LOW);
  delay(time);
}

pb_global_state_t pb_hook_mod_state_read() {
  return puzzleState;
}

void pb_hook_mod_state_write(pb_global_state_t state) {
  puzzleState = state;
}

void pb_hook_ev_main_state_update(pb_global_state_t state){
	Serial.println("WE IN BOISS");
}

void checkButtonPress() {
  int reading = digitalRead(BUTTON_PIN); // read the current state of the button

  if (reading != lastButtonState) {
	Serial.println("Button pressed, changing state to SOLVED.");
	pb_hook_mod_state_write(PB_GS_SOLVED);
    lastDebounceTime = millis();  // reset debouncing timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastButtonState = reading;  // update the last state for next comparison
  }
}

void loop() {
  switch(puzzleState) {
    case PB_GS_PLAYING:
      digitalWrite(EXTERNAL_LED_PIN, LOW);  // LED is off in PLAYING state
	  checkButtonPress();
      break;
    case PB_GS_SOLVED:
      Serial.println("STATE = PB_GS_SOLVED");
      digitalWrite(EXTERNAL_LED_PIN, HIGH);  // LED is on in SOLVED state
      break;
    case PB_GS_NOINIT:
      Serial.println("STATE = PB_GS_NOINIT");
      led_blink(100);  // Blink LED rapidly in NOINIT state
      break;
    case PB_GS_IDLE:
      Serial.println("STATE = PB_GS_IDLE");
      led_blink(500);  // Blink LED slowly in IDLE state
      break;
  }
}
