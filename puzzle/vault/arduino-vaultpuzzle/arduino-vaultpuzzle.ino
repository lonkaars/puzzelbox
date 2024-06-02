#include <Wire.h>
#include <TM1637Display.h>

// Definitions for GPIO numbers, change these according to your hardware setup
#define TOTAL_LEVELS 5
#define TAG "VaultPuzzle"

// Key Matrix Pin Configuration
#define ROWS 4
#define COLS 3

// Module connection pins (Digital Pins for TM1637)
#define CLK 2
#define DIO 3

// Pin to indicate puzzle solved state
#define SOLVED_PIN 53

// Initialize the TM1637 display
TM1637Display display(CLK, DIO);

// I2C slave address
#define I2C_SLAVE_ADDRESS 0x08

// LED on pin 13 to indicate I2C communication
const int ledPin = 13;

const int ROW_PINS[ROWS] = {7, 6, 5, 4};
const int COL_PINS[COLS] = {10, 9, 8};

typedef enum {
    STATE_UNINITIALIZED = 0x00,
    STATE_RESET = 0x01,
    STATE_PLAYING = 0x02,
    STATE_SOLVED = 0x03,
    STATE_ERROR = 0x04
} PuzzleState;

//! TODO: Both codes needs to be genereted using some sort of logic that also match the game manual
const char* validButtons[TOTAL_LEVELS] = {"A2", "B1", "D3", "C2", "C1"};
const char bombCode[] = "1234";  // Display this code at the end

PuzzleState puzzleState = STATE_UNINITIALIZED;
int currentLevel = 0;

// Function prototypes
void display_code(int level);
void display_final_code(const char* code);
void initialize_system();
void check_button_press();
void play_error_sound();
void blink_display(char num);
void receiveEvent(int howMany);

void setup() {
  Serial.begin(115200);
  pinMode(SOLVED_PIN, OUTPUT);
  digitalWrite(SOLVED_PIN, LOW);

  display.setBrightness(0x0f);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Wire.begin(I2C_SLAVE_ADDRESS);
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
    Serial.println("GPIO and display initialized.");
}

void loop() {
  if (puzzleState != STATE_UNINITIALIZED && puzzleState != STATE_ERROR) {
    while (puzzleState != STATE_SOLVED) {
      check_button_press();
      delay(100);
    }
    if (puzzleState == STATE_SOLVED) {
      display_final_code(bombCode);
      digitalWrite(SOLVED_PIN, HIGH);
      Serial.println("Final display shown. Puzzle complete.");
      while (1) { delay(1000); } // Keep displaying final code
    }
  } else if (puzzleState == STATE_ERROR) {
    blink_display('1'); // Blink '1's when in error state
  } else if (puzzleState == STATE_UNINITIALIZED){
    Serial.println("Puzzle module unitialized.");
    blink_display('0'); // Blink '0's when uninitialized
  }
}

void display_final_code(const char* code) {
    for (int i = 0; code[i] != '\0'; i++) {
        display.showNumberDec(code[i] - '0', false); // Display each digit
        delay(1000); // Display each digit for 1 second
    }
}

void display_code(int level) {
    Serial.print("Displaying code for level ");
    Serial.println(level);
    display.showNumberDec(level, true);
    Serial.print("Code for level ");
    Serial.print(level);
    Serial.println(" displayed successfully.");
}

void check_button_press() {
    char keyPress[3] = {0};
    for (int col = 0; col < COLS; col++) {
        digitalWrite(COL_PINS[col], LOW);
        for (int row = 0; row < ROWS; row++) {
            if (digitalRead(ROW_PINS[row]) == LOW) {
                delay(50);
                if (digitalRead(ROW_PINS[row]) == LOW) {
                    keyPress[0] = 'A' + row;
                    keyPress[1] = '1' + col;
                    keyPress[2] = '\0';
                    Serial.print("Keypress detected: ");
                    Serial.println(keyPress);
                    if (strcmp(keyPress, validButtons[currentLevel]) == 0) {
                        currentLevel++;
                        if (currentLevel >= TOTAL_LEVELS) {
                            puzzleState = STATE_SOLVED;
                            Serial.println("Puzzle solved!");
                            display.showNumberDec(currentLevel + 1, true);
                            digitalWrite(SOLVED_PIN, HIGH);
                        } else {
                            puzzleState = STATE_PLAYING;
                            display_code(currentLevel);
                        }
                    } else {
                        puzzleState = STATE_ERROR;
                        currentLevel = 0;
                    }
                    while (digitalRead(ROW_PINS[row]) == LOW) {}
                }
            }
        }
        digitalWrite(COL_PINS[col], HIGH);
    }
}

void play_error_sound() {
    Serial.println("Playing error sound.");
}

void blink_display(char num) {
    if (num == '0') {
        display.showNumberDec(0, true); // Display '0' continuously
        delay(500);
        display.clear(); // Clear display
        delay(500);
    } else if (num == '1') {
        display.showNumberDec(1, true); // Display '1' continuously
        delay(500);
        display.clear(); // Clear display
        delay(500);
    }
}

void receiveEvent(int howMany) {
  if (Wire.available() > 0) {
    char c = Wire.read();
    if (c == 'M' && puzzleState == STATE_UNINITIALIZED) {
      initialize_system();
      puzzleState = STATE_RESET;
      currentLevel = 0;
      display_code(currentLevel);
    }
  }
}

