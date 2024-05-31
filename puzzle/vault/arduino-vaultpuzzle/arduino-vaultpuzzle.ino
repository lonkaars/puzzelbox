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

const char* validButtons[TOTAL_LEVELS] = {"A2", "B1", "D3", "C2", "C1"};
PuzzleState puzzleState = STATE_UNINITIALIZED;
int currentLevel = 0;

// Function prototypes
void display_code(int level);
void initialize_system();
void check_button_press();
void update_state_after_button_press(bool validPress);
void play_error_sound();
void blink_display();
void receiveEvent(int howMany);

void setup() {
  Serial.begin(115200); // Initialize default Serial for debug messages
  pinMode(SOLVED_PIN, OUTPUT);
  digitalWrite(SOLVED_PIN, LOW);

  display.setBrightness(0x0f);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Initialize I2C as a slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent); // Attach receive event handler

  uint8_t allOn[] = {0xFF, 0xFF, 0xFF, 0xFF};
  display.setSegments(allOn);
  delay(2000);
}

void loop() {
  if (puzzleState != STATE_UNINITIALIZED) {
    while (puzzleState != STATE_SOLVED) {
      check_button_press();
      delay(100);
    }
    if (puzzleState == STATE_SOLVED) {
      digitalWrite(SOLVED_PIN, HIGH);
      display.showNumberDec(currentLevel, true);
      Serial.println("Final display shown. Puzzle complete.");
      while (1) { delay(1000); }
    }
  } else {
    delay(100); // Wait until initialized
  }
}

void display_code(int level) {
    Serial.print("Displaying code for level ");
    Serial.println(level);
    display.showNumberDec(level, true); // True to show leading zeros
    Serial.print("Code for level ");
    Serial.print(level);
    Serial.println(" displayed successfully.");
}

void check_button_press() {
    char keyPress[3] = {0};
    for (int col = 0; col < COLS; col++) {
        digitalWrite(COL_PINS[col], LOW); // Activate column
        for (int row = 0; row < ROWS; row++) {
            if (digitalRead(ROW_PINS[row]) == LOW) { // Detect if any row is activated
                delay(50); // Debounce delay
                if (digitalRead(ROW_PINS[row]) == LOW) { // Confirm the button is still pressed
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
                            display.showNumberDec(currentLevel + 1, true); // Display the final level
                            digitalWrite(SOLVED_PIN, HIGH); // Set the solved pin high
                        } else {
                            puzzleState = STATE_PLAYING;
                            display_code(currentLevel);
                        }
                    } else {
                        play_error_sound();
                        blink_display();
                        puzzleState = STATE_ERROR;
                        currentLevel = 0;
                        display_code(currentLevel);
                    }
                    while (digitalRead(ROW_PINS[row]) == LOW) {} // Wait for release
                }
            }
        }
        digitalWrite(COL_PINS[col], HIGH); // Deactivate column
    }
}

void play_error_sound() {
    // Simulate error sound - connect a buzzer to play actual sound
    Serial.println("Playing error sound.");
}

void blink_display() {
    // Simulate blinking the display - use LEDs or other methods to show visual feedback
    Serial.println("7-segment display is blinking to indicate an error.");
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    if (c == 'S') { // Start signal received
      digitalWrite(ledPin, HIGH); // Turn on LED to indicate signal received
      initialize_system();
      puzzleState = STATE_RESET;
      currentLevel = 0;
      display_code(currentLevel);
      digitalWrite(ledPin, LOW); // Turn off LED after initialization
    }
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
