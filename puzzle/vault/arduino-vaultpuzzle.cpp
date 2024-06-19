#include <Wire.h>
#include <TM1637Display.h>

#define TOTAL_LEVELS 5
#define ROWS 4
#define COLS 3
#define CLK 2
#define DIO 3
#define SOLVED_PIN 53
#define I2C_MODULE_ADDRESS 0x08

const int ROW_PINS[ROWS] = {7, 6, 5, 4};
const int COL_PINS[COLS] = {10, 9, 8};
const char* validButtons[TOTAL_LEVELS] = {"A2", "B1", "D3", "C2", "C3"};
const char bombCode[] = "1234";

TM1637Display display(CLK, DIO);

typedef enum {
    STATE_UNINITIALIZED = 0x00,
    STATE_RESET = 0x01,
    STATE_PLAYING = 0x02,
    STATE_SOLVED = 0x03,
    STATE_ERROR = 0x04
} PuzzleState;

PuzzleState puzzleState = STATE_UNINITIALIZED;
int currentLevel = 0;

void setup() {
    Serial.begin(115200);
    pinMode(SOLVED_PIN, OUTPUT);
    digitalWrite(SOLVED_PIN, LOW);
    display.setBrightness(0x0f);
    Wire.begin(I2C_MODULE_ADDRESS);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
    initialize_system();
}

void initialize_system() {
    for (int i = 0; i < ROWS; i++) pinMode(ROW_PINS[i], INPUT_PULLUP);
    for (int i = 0; i < COLS; i++) {
        pinMode(COL_PINS[i], OUTPUT);
        digitalWrite(COL_PINS[i], HIGH);
    }
    display_initial_code();
    Serial.println("GPIO and display initialized.");
}

void loop() {
    if (puzzleState == STATE_UNINITIALIZED) blink_display();
    else if (puzzleState != STATE_ERROR) check_button_press();
    if (puzzleState == STATE_SOLVED) {
        display_final_code(bombCode);
        digitalWrite(SOLVED_PIN, HIGH);
        Serial.println("Final display shown. Puzzle complete.");
    } else if (puzzleState == STATE_ERROR) blink_display_error();
}

void requestEvent() {
    // Example handling for I2C request
}

void receiveEvent(int howMany) {
    // Example handling for I2C receive
}

void blink_display() {
    uint8_t zeroPattern[4] = {
        display.encodeDigit(0), // encode 0 for the first digit
        display.encodeDigit(0), // encode 0 for the second digit
        display.encodeDigit(0), // encode 0 for the third digit
        display.encodeDigit(0)  // encode 0 for the fourth digit
    };
    blink_display_custom(zeroPattern);
}

void blink_display_error() {
    uint8_t errorPattern[4] = {0b01111001, 0b01111001, 0b01111001, 0b01111001}; // 'E' for error
    blink_display_custom(errorPattern);
}

void blink_display_custom(uint8_t segments[]) {
    for (int i = 0; i < 3; i++) {
        display.clear();
        delay(250);
        display.setSegments(segments, 4, 0);
        delay(250);
    }
    display.clear();
}

void display_initial_code() {
    uint8_t segs[4] = {encodeChar(validButtons[currentLevel][0]), encodeChar(validButtons[currentLevel][1]), 0x00, 0x00};
    display.setSegments(segs, 4, 0);
    Serial.println("Initial code displayed.");
}

uint8_t encodeChar(char c) {
    switch (c) {
        case 'A': return 0b01110111;
        case 'B': return 0b01111100;
        case 'C': return 0b00111001;
        case 'D': return 0b01011110;
        case '1': return display.encodeDigit(1);
        case '2': return display.encodeDigit(2);
        case '3': return display.encodeDigit(3);
        default:  return 0x00; // Blank
    }
}

void check_button_press() {
    // Function to check button presses and handle game logic
}

void display_final_code(const char* code) {
    uint8_t segs[4] = {0, 0, 0, 0};
    int numDigits = strlen(code);
    for (int i = 0; i < numDigits; i++) {
        segs[i] = display.encodeDigit(code[i] - '0');
    }
    display.setSegments(segs, numDigits, 0);
}
