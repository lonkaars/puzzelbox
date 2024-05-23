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

//TODO Update these pin numbers based on your Arduino setup
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

void setup() {
  Serial.begin(115200); // Initialize default Serial for debug messages
  pinMode(SOLVED_PIN, OUTPUT); // Initialize the solved indicator pin
  digitalWrite(SOLVED_PIN, LOW); // Start with the solved pin LOW

  display.setBrightness(0x0f); // Set the brightness of the TM1637 display
  initialize_system();
  Serial.println("GPIO and display initialized.");

  // Test to light up all segments
  uint8_t allOn[] = {0xFF, 0xFF, 0xFF, 0xFF}; // All segments on
  display.setSegments(allOn);
  delay(2000); // Keep it on for 2 seconds before proceeding

  // Initialize the game
  if (true) { // Simulating isVaultClosed
      puzzleState = STATE_RESET;
      currentLevel = 0;
      display_code(currentLevel);
  } else {
      Serial.println("Vault door is open. Please close the door to start the puzzle.");
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

void loop() {
  while (puzzleState != STATE_SOLVED) {
      check_button_press();
      delay(100); // Non-blocking delay
  }
  // When puzzle is solved, you might want to display a final message and set the solved pin high
  if (puzzleState == STATE_SOLVED) {
      digitalWrite(SOLVED_PIN, HIGH); // Set the solved pin high
      display.showNumberDec(currentLevel, true); // Show final level or a special message
      Serial.println("Final display shown. Puzzle complete.");
      while (1) { delay(1000); } // Hold on the final display
  }
}

void display_code(int level) {
    Serial.print("Displaying code for level "); Serial.println(level);
    // Display the level on the TM1637 4-digit 7-segment display
    display.showNumberDec(level, true); // True to show leading zeros
    Serial.print("Code for level "); Serial.print(level); Serial.println(" displayed successfully.");
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
                    Serial.print("Keypress detected: "); Serial.println(keyPress);
                    if (strcmp(keyPress, validButtons[currentLevel]) == 0) {
                        currentLevel++;
                        if (currentLevel >= TOTAL_LEVELS) {
                            puzzleState = STATE_SOLVED;
                            Serial.println("Puzzle solved!");
                            display.showNumberDec(currentLevel + 1, true);  // Display the final level
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
