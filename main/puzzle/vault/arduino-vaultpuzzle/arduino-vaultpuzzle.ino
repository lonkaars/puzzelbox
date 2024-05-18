#include <Wire.h>

// Definitions for GPIO numbers, change these according to your hardware setup
#define TOTAL_LEVELS 5
#define TAG "VaultPuzzle"

// Key Matrix Pin Configuration
#define ROWS 4
#define COLS 3
const int ROW_PINS[ROWS] = {32, 33, 25, 26}; // Update these pin numbers based on your Arduino setup
const int COL_PINS[COLS] = {27, 14, 12}; // Update these pin numbers based on your Arduino setup

typedef enum {
    STATE_UNINITIALIZED,
    STATE_RESET,
    STATE_PLAYING,
    STATE_SOLVED,
    STATE_ERROR
} PuzzleState;

const char* validButtons[TOTAL_LEVELS] = {"A3", "F1", "U4", "C2", "L1"};
PuzzleState puzzleState = STATE_UNINITIALIZED;
int currentLevel = 0;

// Function prototypes
void send_i2c_update(PuzzleState state);
void display_code(int level);
void initialize_system();
void check_button_press();
void update_state_after_button_press(bool validPress);

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C as master
  initialize_system();
  Serial.println("GPIO and I2C initialized.");
}

void loop() {
  while (puzzleState != STATE_SOLVED) {
      check_button_press();
      delay(100); // Non-blocking delay
  }
}

void send_i2c_update(PuzzleState state) {
    uint8_t data;
    switch (state) {
        case STATE_UNINITIALIZED: data = 0x00; break;
        case STATE_RESET: data = 0x01; break;
        case STATE_PLAYING: data = 0x02; break;
        case STATE_SOLVED: data = 0x03; break;
        case STATE_ERROR: data = 0x04; break;
        default: data = 0xFF; // Unknown state
    }
    Serial.print("Sending state "); Serial.println(state);
    
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write(data);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.print("State update sent successfully.");
    } else {
        Serial.print("Failed to send state update via I2C.");
    }
}

void display_code(int level) {
    Serial.print("Displaying code for level "); Serial.println(level);
    
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write(level);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.print("Code for level "); Serial.print(level); Serial.println(" displayed successfully.");
    } else {
        Serial.print("Failed to display code for level "); Serial.print(level); Serial.println(" via I2C.");
    }
}

void initialize_system() {
    // Configure the rows as input with pull-up
    for (int i = 0; i < ROWS; i++) {
      pinMode(ROW_PINS[i], INPUT_PULLUP);
    }
    
    // Configure the columns as output
    for (int i = 0; i < COLS; i++) {
      pinMode(COL_PINS[i], OUTPUT);
      digitalWrite(COL_PINS[i], HIGH);
    }
}

void check_button_press() {
    char keyPress[3] = {0};
    for (int col = 0; col < COLS; col++) {
        digitalWrite(COL_PINS[col], LOW); // Activate column
        for (int row = 0; row < ROWS; row++) {
            if (digitalRead(ROW_PINS[row]) == LOW) { // Detect if any row is activated
                keyPress[0] = 'A' + row;
                keyPress[1] = '1' + col;
                keyPress[2] = '\0';
                Serial.print("Keypress detected: "); Serial.println(keyPress);
                update_state_after_button_press(strcmp(keyPress, validButtons[currentLevel]) == 0);
                while (digitalRead(ROW_PINS[row]) == LOW) {} // Wait for release
            }
        }
        digitalWrite(COL_PINS[col], HIGH); // Deactivate column
    }
}

void update_state_after_button_press(bool validPress) {
    if (validPress) {
        if (currentLevel >= TOTAL_LEVELS) {
            puzzleState = STATE_SOLVED;
            Serial.println("Puzzle solved!");
            send_i2c_update(puzzleState);
        } else {
            puzzleState = STATE_PLAYING;
            currentLevel++;
            display_code(currentLevel);
        }
    } else {
        puzzleState = STATE_ERROR;
        Serial.println("Error in input");
    }
    send_i2c_update(puzzleState);
}
