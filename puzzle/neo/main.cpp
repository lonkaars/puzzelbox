#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoTrellis.h>

#define MATRIX_SIZE 8
#define LED_COLOR_ON 0xFFFFFF // Color of the LEDs in ON state
#define LED_COLOR_OFF 0x000000 // Color of the LEDs in OFF state

Adafruit_NeoTrellis t_array[MATRIX_SIZE / 4][MATRIX_SIZE / 4] = {
    {Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F)},
    {Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32)}
};

Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, MATRIX_SIZE / 4, MATRIX_SIZE / 4);

bool neoMatrix[MATRIX_SIZE][MATRIX_SIZE]; // To track state of each pixel

enum NeoState {
    NEO_UNINITIALIZED,
    NEO_PLAYING,
    NEO_SOLVED
};

NeoState neoState = NEO_UNINITIALIZED;

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
            neoState = NEO_SOLVED;
            Serial.println("The NeoTrellis puzzle is solved!");
        }
    }
    return 0;
}


void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial to be ready

    if (!trellis.begin()) {
        Serial.println("Failed to initialize NeoTrellis");
        while (1) delay(1);
    }

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
    neoState = NEO_PLAYING;

    // Register the callback for each key
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING, true);
        trellis.registerCallback(i, buttonCallback);
    }
}

void loop() {
    trellis.read(); // Process button events
    delay(20);
}