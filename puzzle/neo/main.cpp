#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoTrellis.h>
#include "lib/pbdrv/pb-types.h"
#include "lib/pbdrv/pb-mod.h"

#define MATRIX_SIZE 8
#define LED_COLOR_ON 0x0000FF // Color of the LEDs in ON state
#define LED_COLOR_OFF 0x000000 // Color of the LEDs in OFF state
#define LED_COLOR_RED 0xFF0000 // Red color for UNINIT state
#define LED_COLOR_ORANGE 0xFFA500 // Orange color for IDLE state

Adafruit_NeoTrellis t_array[MATRIX_SIZE / 4][MATRIX_SIZE / 4] = {
	{Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F)},
	{Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32)}
};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, MATRIX_SIZE / 4, MATRIX_SIZE / 4);

bool neoMatrix[MATRIX_SIZE][MATRIX_SIZE]; // To track state of each pixel


unsigned long previousMillis = 0;
const long interval = 500; // Interval at which to blink (milliseconds)

bool gamefield = false;
bool ledState = false;

// Puzzle state
pb_global_state_t puzzleState = PB_GS_NOINIT;

void toggleAdjacentLEDs(int x, int y) {
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0) continue; // Skip the center button itself
			int nx = x + dx, ny = y + dy;
			if (nx >= 0 && nx < MATRIX_SIZE && ny >= 0 && ny < MATRIX_SIZE) {
				neoMatrix[nx][ny] = !neoMatrix[nx][ny];
				trellis.setPixelColor(nx * MATRIX_SIZE + ny, neoMatrix[nx][ny] ? LED_COLOR_ON : LED_COLOR_OFF);
			}
		}
	}
}


bool isNeoPuzzleSolved() {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			if (neoMatrix[i][j]) return false; // If any LED is on, puzzle is not solved
		}
	}
	return true;
}

TrellisCallback buttonCallback(keyEvent evt) {
	int x = evt.bit.NUM / MATRIX_SIZE;
	int y = evt.bit.NUM % MATRIX_SIZE;

	if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
		toggleAdjacentLEDs(x, y);
		trellis.show();
		if (isNeoPuzzleSolved()) {
			pb_hook_mod_state_write(PB_GS_SOLVED);
			Serial.println("The NeoTrellis puzzle is solved!");
		}
	}
	return 0;
}

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for Serial to be read
	if (!trellis.begin()) {
		Serial.println("Failed to initialize NeoTrellis");
		pb_hook_mod_state_write(PB_GS_NOINIT);
	}
}

void set_game_field() {
	if (gamefield == false){
		// Initialize the matrix with a checkerboard pattern
		bool toggle = false;
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				neoMatrix[i][j] = toggle;
				toggle = !toggle;
				trellis.setPixelColor(i * MATRIX_SIZE + j, neoMatrix[i][j] ? LED_COLOR_ON : LED_COLOR_OFF);
			}
			toggle = !toggle;
		}
		trellis.show();

		// Register the callback for each key
		for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
			trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
			trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING, true);
			trellis.registerCallback(i, buttonCallback);
		}
		gamefield = true;
	}
}

pb_global_state_t pb_hook_mod_state_read() {
	return puzzleState;
}

void pb_hook_mod_state_write(pb_global_state_t state) {
	puzzleState = state;
}

void flashCorners(uint32_t color) {
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;
		ledState = !ledState;

		for (int i = 0; i < MATRIX_SIZE / 4; i++) {
			for (int j = 0; j < MATRIX_SIZE / 4; j++) {
				int baseIndex = (i * 4 * MATRIX_SIZE) + (j * 4);
				if (ledState) {
					trellis.setPixelColor(baseIndex, color); // Top-left corner
					trellis.setPixelColor(baseIndex + 3, color); // Top-right corner
					trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE, color); // Bottom-left corner
					trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE + 3, color); // Bottom-right corner
				} else {
					trellis.setPixelColor(baseIndex, LED_COLOR_OFF);
					trellis.setPixelColor(baseIndex + 3, LED_COLOR_OFF);
					trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE, LED_COLOR_OFF);
					trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE + 3, LED_COLOR_OFF);
				}
			}
		}
		trellis.show();
	}
}

void loop() {
	switch(puzzleState) {
		case PB_GS_PLAYING:
			set_game_field();
			trellis.read(); // Process button events
			delay(20);
			break;
		case PB_GS_SOLVED:
			Serial.println("STATE = PB_GS_SOLVED");
			break;
		case PB_GS_NOINIT:
			Serial.println("STATE = PB_GS_NOINIT");
			flashCorners(LED_COLOR_RED);
			break;
		case PB_GS_IDLE:
			Serial.println("STATE = PB_GS_IDLE");
			flashCorners(LED_COLOR_ORANGE);
			break;
	}
}

