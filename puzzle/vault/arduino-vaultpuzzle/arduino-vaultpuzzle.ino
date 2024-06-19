#include <Wire.h>
#include <TM1637Display.h>

#define TOTAL_LEVELS 5
#define ROWS 4
#define COLS 3
#define CLK 2
#define DIO 3
#define SOLVED_PIN 53
#define I2C_MODULE_ADDRESS 0x08
#define HANDSHAKE_RECEIVED {0x70, 0x75, 0x7a, 0x62, 0x75, 0x73}
#define HANDSHAKE_SEND {0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67}
#define REQUEST_STATE_CMD 0x53

const int ROW_PINS[ROWS] = {7, 6, 5, 4};
const int COL_PINS[COLS] = {10, 9, 8};
const char* levelCodes[TOTAL_LEVELS] = {"A2", "B1", "D3", "C2", "C3"}; // Predefined valid codes per level
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
}

void initialize_system() {
    for (int i = 0; i < ROWS; i++) {
      pinMode(ROW_PINS[i], INPUT_PULLUP);
    }
    for (int i = 0; i < COLS; i++) {
      pinMode(COL_PINS[i], OUTPUT);
      digitalWrite(COL_PINS[i], HIGH);
    }
    display_code(currentLevel);  // Display initial code for level 0
    Serial.println("GPIO and display initialized.");
}

void loop() {
    if (puzzleState == STATE_PLAYING) {
        check_button_press();
        delay(100);
    }
    if (puzzleState == STATE_SOLVED) {
        display_final_code(bombCode);
        digitalWrite(SOLVED_PIN, HIGH);
        Serial.println("Final display shown. Puzzle complete.");
    } else if (puzzleState == STATE_ERROR) {
        blink_display('1');
    } else if (puzzleState == STATE_UNINITIALIZED) {
        blink_display('0');
    }
}

void display_code(int level) {
    // Display a letter and a digit as a code for the current level
    if (level < TOTAL_LEVELS) {
        char codeToDisplay[3] = {levelCodes[level][0], levelCodes[level][1], '\0'};
        Serial.print("Displaying code for level ");
        Serial.println(level);
        display.showString(codeToDisplay);
        Serial.print("Code for level ");
        Serial.print(level);
        Serial.println(" displayed successfully.");
    }
}

void check_button_press() {
    for (int col = 0; col < COLS; col++) {
        digitalWrite(COL_PINS[col], LOW);
        for (int row = 0; row < ROWS; row++) {
            if (digitalRead(ROW_PINS[row]) == LOW) {
                delay(50); // Debounce delay
                if (digitalRead(ROW_PINS[row]) == LOW) {
                    char keyPress[3] = {'A' + row, '1' + col, '\0'};
                    Serial.print("Keypress detected: ");
                    Serial.println(keyPress);
                    if (strcmp(keyPress, levelCodes[currentLevel]) == 0) {
                        currentLevel++;
                        if (currentLevel >= TOTAL_LEVELS) {
                            puzzleState = STATE_SOLVED;
                        } else {
                            puzzleState = STATE_PLAYING;
                            display_code(currentLevel);
                        }
                    } else {
                        currentLevel = 0;
                        blink_display_error();
                        display_code(currentLevel); // Re-display the correct code after error
                    }
                    while (digitalRead(ROW_PINS[row]) == LOW) {} // Wait for button release
                }
            }
        }
        digitalWrite(COL_PINS[col], HIGH);
    }
}

void blink_display_error() {
    for (int i = 0; i < 3; i++) { // Blink 3 times on error
        display.clear();
        delay(250);
        display.showString("Err");
        delay(250);
    }
    display.clear();
}
