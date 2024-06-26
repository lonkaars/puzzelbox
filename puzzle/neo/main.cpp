#include "pb-mod.h"
#include "pb-types.h"
#include "pb.h"
#include <Adafruit_NeoTrellis.h>
#include <Arduino.h>
#include <Wire.h>

#define MATRIX_SIZE 8
#define LED_COLOR_ON 0x0000FF // Color of the LEDs in ON state
#define LED_COLOR_OFF 0x000000 // Color of the LEDs in OFF state
#define LED_COLOR_RED 0xFF0000 // Red color for UNINIT state
#define LED_COLOR_ORANGE 0xFFA500 // Orange color for IDLE state
#define brightness 0.3 // Set brightness

Adafruit_NeoTrellis t_array[MATRIX_SIZE / 4][MATRIX_SIZE / 4] = {
	{Adafruit_NeoTrellis(PB_ADDR_ADA_NEO_1),
	 Adafruit_NeoTrellis(PB_ADDR_ADA_NEO_2)},
	{Adafruit_NeoTrellis(PB_ADDR_ADA_NEO_3),
	 Adafruit_NeoTrellis(PB_ADDR_ADA_NEO_4)},
};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *) t_array, MATRIX_SIZE / 4,
							  MATRIX_SIZE / 4);

bool neoMatrix[MATRIX_SIZE][MATRIX_SIZE]; // To track state of each pixel

unsigned long previousMillis = 0;
const long interval = 500; // Interval at which to blink (milliseconds)

bool gamefield = false;
bool ledState = false;

// Puzzle state
pb_global_state_t puzzleState = PB_GS_NOINIT;

/**
 * Scales the brightness of a color.
 *
 * @param color The color to be scaled.
 * @param scale The scale factor.
 *
 * @return The scaled color.
 *
 * @throws None.
 */
uint32_t scaleBrightness(uint32_t color, float scale) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    r = uint8_t(r * scale);
    g = uint8_t(g * scale);
    b = uint8_t(b * scale);

    return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
}

/**
 * Toggles the state of adjacent LEDs based on the input coordinates.
 *
 * @param x The x-coordinate of the LED to toggle.
 * @param y The y-coordinate of the LED to toggle.
 *
 * @throws None
 */
void toggleAdjacentLEDs(int x, int y) {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < MATRIX_SIZE && ny >= 0 && ny < MATRIX_SIZE) {
                neoMatrix[nx][ny] = !neoMatrix[nx][ny];  // Toggle each adjacent LED
                uint32_t color = neoMatrix[nx][ny] ? LED_COLOR_ON : LED_COLOR_OFF;
                trellis.setPixelColor(nx * MATRIX_SIZE + ny, scaleBrightness(color, brightness));
            }
        }
    }
}


/**
 * Checks if the NeoPuzzle is solved.
 *
 * @return true if all LEDs in the NeoPuzzle are off, false otherwise.
 *
 * @throws None
 */
bool isNeoPuzzleSolved() {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			if (neoMatrix[i][j])
				return false; // If any LED is on, puzzle is not solved
		}
	}
	return true;
}

/**
 * Callback function triggered by a Trellis button press event.
 *
 * @param evt the key event containing information about the button press
 *
 * @return 0 indicating the success of the callback
 *
 * @throws None
 */
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

/**
 * Initializes the serial communication and checks if the NeoTrellis board is initialized successfully.
 * If the initialization fails, it sets the module state to PB_GS_NOINIT and prints an error message.
 * Finally, it sets the module state to PB_GS_PLAYING.
 *
 * @throws None
 */
void setup() {
	Serial.begin(115200);
	while (!Serial)
		; // Wait for Serial to be read
	if (!trellis.begin()) {
		Serial.println("Failed to initialize NeoTrellis");
		pb_hook_mod_state_write(PB_GS_NOINIT);
	}
	pb_hook_mod_state_write(PB_GS_PLAYING);
}

/**
 * Initializes the game field based on the current state of the NeoMatrix.
 *
 * @return None
 *
 * @throws None
 */
void set_game_field() {
    if (!gamefield) {
        bool toggle = false;
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                neoMatrix[i][j] = toggle;
                uint32_t color = neoMatrix[i][j] ? LED_COLOR_ON : LED_COLOR_OFF;
                trellis.setPixelColor(i * MATRIX_SIZE + j, scaleBrightness(color, brightness));
                toggle = !toggle;
            }
            toggle = !toggle;
        }
        trellis.show();

        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
            trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
            trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING, true);
            trellis.registerCallback(i, buttonCallback);
        }
        gamefield = true;
    }
}


pb_global_state_t pb_hook_mod_state_read() { return puzzleState; }

void pb_hook_mod_state_write(pb_global_state_t state) { puzzleState = state; }

/**
 * Function to flash the corners of a matrix with a specified color.
 *
 * @param color The color value to be used for flashing.
 *
 * @return None
 *
 * @throws None
 */
void flashCorners(uint32_t color) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        color = scaleBrightness(color, brightness); // Scale color brightness

        for (int i = 0; i < MATRIX_SIZE / 4; i++) {
            for (int j = 0; j < MATRIX_SIZE / 4; j++) {
                int baseIndex = (i * 4 * MATRIX_SIZE) + (j * 4);
                if (ledState) {
                    trellis.setPixelColor(baseIndex, color);
                    trellis.setPixelColor(baseIndex + 3, color);
                    trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE, color);
                    trellis.setPixelColor(baseIndex + 3 * MATRIX_SIZE + 3, color);
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

/**
 * This function handles the different states of the puzzle game. 
 * It reads button events and updates the game field accordingly.
 *
 * @return void
 *
 * @throws None
 */
void loop() {
	switch (puzzleState) {
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
