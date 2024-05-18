#include <Wire.h>
#include <Adafruit_NeoTrellis.h>

#define MATRIX_SIZE 8
#define INT_PIN 5 // Interrupt pin for the NeoTrellis

enum NeoState {
    NEO_UNINITIALIZED,
    NEO_PLAYING,
    NEO_SOLVED
};

Adafruit_NeoTrellis trellis;
NeoState neoState = NEO_UNINITIALIZED;

// Initialize the NeoTrellis matrix
void initializeNeoMatrix() {
    if (!trellis.begin()) {
        Serial.println("Failed to initialize NeoTrellis");
        while (1); // Hold here if initialization fails
    }

    // Set all buttons to listen for presses and releases
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            trellis.activateKey(i * MATRIX_SIZE + j, SEESAW_KEYPAD_EDGE_RISING, true);
            trellis.activateKey(i * MATRIX_SIZE + j, SEESAW_KEYPAD_EDGE_FALLING, true);
            trellis.setPixelColor(i * MATRIX_SIZE + j, 0x000000); // Turn off LED
        }
    }
    trellis.show();
    neoState = NEO_PLAYING;
}

// Callback to handle button presses
void buttonCallback(uint8_t x) {
    uint8_t i = x / MATRIX_SIZE;
    uint8_t j = x % MATRIX_SIZE;

    // Toggle the central button and adjacent LEDs
    toggleAdjacentLEDs(i, j);
    if (isNeoPuzzleSolved()) {
        neoState = NEO_SOLVED;
        Serial.println("The NeoTrellis puzzle is solved!");
        // Additional actions upon solving the puzzle can go here
    }
    trellis.show();
}

void toggleAdjacentLEDs(int x, int y) {
    int idx = x * MATRIX_SIZE + y;
    trellis.setPixelColor(idx, trellis.getPixelColor(idx) ^ 0xFFFFFF); // Toggle LED color

    // Toggle adjacent LEDs
    if (x > 0) trellis.setPixelColor((x-1) * MATRIX_SIZE + y, trellis.getPixelColor((x-1) * MATRIX_SIZE + y) ^ 0xFFFFFF);
    if (x < MATRIX_SIZE - 1) trellis.setPixelColor((x+1) * MATRIX_SIZE + y, trellis.getPixelColor((x+1) * MATRIX_SIZE + y) ^ 0xFFFFFF);
    if (y > 0) trellis.setPixelColor(x * MATRIX_SIZE + (y-1), trellis.getPixelColor(x * MATRIX_SIZE + (y-1)) ^ 0xFFFFFF);
    if (y < MATRIX_SIZE - 1) trellis.setPixelColor(x * MATRIX_SIZE + (y+1), trellis.getPixelColor(x * MATRIX_SIZE + (y+1)) ^ 0xFFFFFF);
}

bool isNeoPuzzleSolved() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (trellis.getPixelColor(i * MATRIX_SIZE + j) != 0x000000) return false; // If any LED is on, puzzle is not solved
        }
    }
    return true;
}

void setup() {
    Serial.begin(115200);
    trellis.begin(INT_PIN);
    trellis.setBrightness(50); // Set brightness of LEDs (0-255)
    initializeNeoMatrix();
    trellis.registerCallback(buttonCallback);
}

void loop() {
    if (neoState == NEO_PLAYING) {
        if (trellis.read()) { // If there was a button event
            trellis.show(); // Update the display
        }
    }
}
