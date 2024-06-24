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


// Puzzle state
pb_global_state_t puzzleState = PB_GS_NOINIT;

TM1637Display display(CLK, DIO);

int currentLevel = 0;

void blink_display(char num) {
    while(puzzleState == PB_GS_NOINIT) {
        display.showNumberDecEx(0, 0b11111111, true); // Display "0000" with all digits on
        delay(500);
        display.clear();
        delay(500);
    }
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
                            puzzleState = PB_GS_SOLVED;
                            Serial.println("Puzzle solved!");
                            display.showNumberDec(currentLevel + 1, true);
                            digitalWrite(SOLVED_PIN, HIGH);
                        } else {
                            puzzleState = PB_GS_PLAYING;
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


void setup() {
    Serial.begin(115200);
    pinMode(SOLVED_PIN, OUTPUT);
    digitalWrite(SOLVED_PIN, LOW);
    display.setBrightness(0x0f);

    //puzzelState = pb_hook_mod_state_read();
    

    ///// TEST
    puzzleState = PB_GS_IDLE;
    initialize_system();

    // Initialize display with blinking zeros to indicate no connection or initialization state
    blink_display('0');
}

void loop() {
    switch(puzzleState) {
        case PB_GS_PLAYING:
            check_button_press();
            delay(100);
            break;
        case PB_GS_SOLVED:
            display_final_code(bombCode);
            digitalWrite(SOLVED_PIN, HIGH);
            Serial.println("Final display shown. Puzzle complete.");
            break;
        case PB_GS_NOINIT:
            blink_display('0');
            break;
        case PB_GS_IDLE:
            blink_display('1');
            break;
    }
    //puzzelState = pb_hook_mod_state_read();
}