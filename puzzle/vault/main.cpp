#include <Arduino.h>
#include <TM1637Display.h>
#include "lib/pbdrv/pb-types.h"
#include "lib/pbdrv/pb-mod.h"

#define TOTAL_LEVELS 5
#define ROWS 4
#define COLS 3
#define CLK 2
#define DIO 3
#define SOLVED_PIN 53
#define I2C_MODULE_ADDRESS 0x08  // Address of the puzzle module
#define HANDSHAKE_RECEIVED {0x70, 0x75, 0x7a, 0x62, 0x75, 0x73}   // Magic command for the handshake
#define HANDSHAKE_SEND {0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67}      // Magic command for the handshake
#define REQUEST_STATE_CMD 0x53  // 'S' to request the game state

const int ROW_PINS[ROWS] = {7, 6, 5, 4};
const int COL_PINS[COLS] = {10, 9, 8};
const char* validButtons[TOTAL_LEVELS] = {"A2", "B1", "D3", "C2", "C3"};
const char bombCode[] = "1234";
const uint8_t SEGMENT_MAP[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001  // F
    // Add other letters if needed
};

// This array of level codes matches the codes you might display per level.
const char* levelCodes[TOTAL_LEVELS] = {"A1", "B2", "D1", "C3", "A2"};


// Puzzle state
pb_global_state_t puzzleState = PB_GS_NOINIT;

TM1637Display display(CLK, DIO);

int currentLevel = 0;

void blink_display(int num) {
    if (num == 1) {
        // Display "1111" with leading zeros shown if necessary
        display.showNumberDecEx(1111, 0b11111111, true);
    } else if (num == 0) {
        // Display "0000" with leading zeros shown if necessary
        display.showNumberDecEx(0, 0b11111111, true);
    }
    delay(500);
    display.clear();
    delay(500);
}


void display_final_code(const char* code) {
    uint8_t segs[4] = {0, 0, 0, 0}; 
    int numDigits = strlen(code);   
    numDigits = numDigits > 4 ? 4 : numDigits; 

    for (int i = 0; i < numDigits; i++) {
        segs[i] = display.encodeDigit(code[i] - '0');
    }

    display.setSegments(segs, numDigits, 0);
}

void check_button_press() {
    for (int col = 0; col < COLS; col++) {
        digitalWrite(COL_PINS[col], LOW);
        for (int row = 0; row < ROWS; row++) {
            if (digitalRead(ROW_PINS[row]) == LOW) {
                delay(50);
                if (digitalRead(ROW_PINS[row]) == LOW) {
                    char keyPress[3] = {'A' + row, '1' + col, '\0'};
                    Serial.print("Keypress detected: ");
                    Serial.println(keyPress);
                    if (strcmp(keyPress, validButtons[currentLevel]) == 0) {
                        currentLevel++;
                        if (currentLevel >= TOTAL_LEVELS) {
                            pb_hook_mod_state_write(PB_GS_SOLVED);
                            Serial.println("Puzzle solved!");
                            display.showNumberDec(currentLevel + 1, true);
                            digitalWrite(SOLVED_PIN, HIGH);
                        }
                    } else {
                        currentLevel = 0;
                    }
                    while (digitalRead(ROW_PINS[row]) == LOW) {} // Ensure button release
                }
            }
        }
        digitalWrite(COL_PINS[col], HIGH);
    }
}

void initialize_system() {
    for (int i = 0; i < ROWS; i++) {
        pinMode(ROW_PINS[i], INPUT_PULLUP);
    }
    for (int i = 0; i < COLS; i++) {
        pinMode(COL_PINS[i], OUTPUT);
        digitalWrite(COL_PINS[i], HIGH);
    }
    Serial.println("GPIO and display initialized.");
}

void display_code_for_level(int level) {
    char code[3] = {0}; // Temp storage for level code
    strncpy(code, levelCodes[level], 2); // Copy the level-specific code

    uint8_t segs[4] = {0}; // Segments to send to the display

    // Check if the first character is a letter and map it
    if (isalpha(code[0])) {
        if (code[0] >= 'A' && code[0] <= 'F') {
            segs[0] = SEGMENT_MAP[code[0] - 'A' + 10]; // Maps A-F to their segment patterns
        } else {
            // Handle unexpected characters or extend SEGMENT_MAP for more letters
            segs[0] = 0; // Display nothing for undefined letters
        }
    } else {
        // Assume it's a number and map directly
        segs[0] = SEGMENT_MAP[code[0] - '0'];
    }

    // Check if the second character is a digit and map it
    if (isdigit(code[1])) {
        segs[1] = SEGMENT_MAP[code[1] - '0'];
    } else {
        // Handle unexpected characters
        segs[1] = 0; // Display nothing for undefined digits
    }

    // Set only the first two segments, leave others blank
    display.setSegments(segs, 2, 0); // Display on leftmost two digits
}


pb_global_state_t pb_hook_mod_state_read() {
	return puzzleState;
}

void pb_hook_mod_state_write(pb_global_state_t state) {
	puzzleState = state;
}

void setup() {
    Serial.begin(115200);
    pinMode(SOLVED_PIN, OUTPUT);
    digitalWrite(SOLVED_PIN, LOW);
    display.setBrightness(0x0f);  
    initialize_system();
}

void loop() {
    switch(puzzleState) {
        case PB_GS_PLAYING:
            display_code_for_level(currentLevel);
            check_button_press();
            delay(100);
            break;
        case PB_GS_SOLVED:
            Serial.println("STATE = PB_GS_SOLVED");
            display_final_code(bombCode);
            digitalWrite(SOLVED_PIN, HIGH);
            break;
        case PB_GS_NOINIT:
            Serial.println("STATE = PB_GS_NOINIT");
            blink_display(0);
            break;
        case PB_GS_IDLE:
            Serial.println("STATE = PB_GS_IDLE");
            blink_display(1);
            break;
    }
}