#include <Wire.h>
#include <Adafruit_NeoTrellis.h>

#define MATRIX_SIZE 8
#define INT_PIN 5 // Interrupt pin for the NeoTrellis
#define LED_COLOR_ON 0xFFFFFF // Color of the LEDs in ON state
#define LED_COLOR_OFF 0x000000 // Color of the LEDs in OFF state

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
            trellis.pixels.setPixelColor(i * MATRIX_SIZE + j, LED_COLOR_OFF); // Turn off LED
        }
    }
    trellis.pixels.show();
    neoState = NEO_PLAYING;
}

// Callback to handle button presses
void buttonCallback(keyEvent evt) {
    uint8_t i = evt.bit.NUM / MATRIX_SIZE;
    uint8_t j = evt.bit.NUM % MATRIX_SIZE;

    // Toggle the central button and adjacent LEDs
    toggleAdjacentLEDs(i, j);
    if (isNeoPuzzleSolved()) {
        neoState = NEO_SOLVED;
        Serial.println("The NeoTrellis puzzle is solved!");
        // Additional actions upon solving the puzzle can go here
    }
    trellis.pixels.show();
}

void toggleAdjacentLEDs(int x, int y) {
    int idx = x * MATRIX_SIZE + y;
    trellis.pixels.setPixelColor(idx, trellis.pixels.getPixelColor(idx) ^ LED_COLOR_ON); // Toggle LED color

    // Toggle adjacent LEDs
    if (x > 0) trellis.pixels.setPixelColor((x-1) * MATRIX_SIZE + y, trellis.pixels.getPixelColor((x-1) * MATRIX_SIZE + y) ^ LED_COLOR_ON);
    if (x < MATRIX_SIZE - 1) trellis.pixels.setPixelColor((x+1) * MATRIX_SIZE + y, trellis.pixels.getPixelColor((x+1) * MATRIX_SIZE + y) ^ LED_COLOR_ON);
    if (y > 0) trellis.pixels.setPixelColor(x * MATRIX_SIZE + (y-1), trellis.pixels.getPixelColor(x * MATRIX_SIZE + (y-1)) ^ LED_COLOR_ON);
    if (y < MATRIX_SIZE - 1) trellis.pixels.setPixelColor(x * MATRIX_SIZE + (y+1), trellis.pixels.getPixelColor(x * MATRIX_SIZE + (y+1)) ^ LED_COLOR_ON);
}

bool isNeoPuzzleSolved() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (trellis.pixels.getPixelColor(i * MATRIX_SIZE + j) != LED_COLOR_OFF) return false; // If any LED is on, puzzle is not solved
        }
    }
    return true;
}

// Declare a wrapper function that will call your actual callback
void buttonCallbackWrapper(keyEvent evt) {
    buttonCallback(evt);
}

// Adjust the toTrellisCallback function to directly return the wrapper
TrellisCallback toTrellisCallback(void (*callback)(keyEvent)) {
    return buttonCallbackWrapper;
}

void setup() {
    Serial.begin(115200);
    trellis.begin(INT_PIN);
    trellis.pixels.setBrightness(50); // Set brightness of LEDs (0-255)
    initializeNeoMatrix();

    // Register the callback for each key
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        // Directly use the wrapper function here as the callback is static and does not need conversion
        trellis.registerCallback(i, buttonCallbackWrapper);
    }
}


void loop() {
    if (neoState == NEO_PLAYING) {
        trellis.read(); // Handle any button events
        trellis.pixels.show(); // Update the display
    }
}
