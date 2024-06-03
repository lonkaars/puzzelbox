#include <Wire.h>
#include <TM1637Display.h>

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

//! TODO
// Only the handshake should use master-slave logic
// States use master-master logic (no request, just writes to correct address)


//! TODO: Both codes below need to be generated using some sort of logic so that it matches the outcome of the game manual
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
        }
    } else if (puzzleState == STATE_ERROR) {
        blink_display('1');
    } else if (puzzleState == STATE_UNINITIALIZED) {
        blink_display('0');
    }
}

void requestEvent() {
    if (puzzleState == STATE_PLAYING) {
        uint8_t responseData[] = HANDSHAKE_SEND;
        Wire.write(responseData, sizeof(responseData));
        Serial.println("Handshake response sent.");
    }
    // } else {
    //     // Optionally handle other states if needed, send different data or handle errors
    //     Wire.write((uint8_t)STATE_UNINITIALIZED);
    // }
}

void receiveEvent(int howMany) {
    if (howMany == 6) {
        uint8_t expectedBytes[] = HANDSHAKE_RECEIVED;
        uint8_t receivedBytes[6];
        bool match = true;

        for (int i = 0; i < 6; i++) {
            receivedBytes[i] = Wire.read();
            if (receivedBytes[i] != expectedBytes[i]) {
                match = false;
                break;
            }
        }

        if (match) {
            Serial.println("Correct handshake data received.");
            puzzleState = STATE_PLAYING;
            initialize_system();
        } else {
            Serial.println("Incorrect handshake data received.");
            puzzleState = STATE_ERROR;
        }
    } else {
        Serial.print("Received wrong number of bytes: ");
        Serial.println(howMany);
        puzzleState = STATE_ERROR;
    }
}




void display_final_code(const char* code) {
    uint8_t segs[4] = {0, 0, 0, 0}; // Array to hold the segment data for each digit
    int numDigits = strlen(code);   // Get the length of the code
    numDigits = numDigits > 4 ? 4 : numDigits; // Ensure we don't exceed display capacity

    for (int i = 0; i < numDigits; i++) {
        if (code[i] >= '0' && code[i] <= '9') {
            // Convert character to appropriate segment code for a 7-segment display
            segs[i] = display.encodeDigit(code[i] - '0');
        } else {
            // Handle non-digit characters appropriately, or set to 0
            // You would need a function or lookup table to convert characters to segment codes
            segs[i] = 0; // Placeholder for non-digit handling
        }
    }

    // Set the segments; assuming 'true' for colon display, adjust according to your display's capabilities
    display.setSegments(segs, numDigits, 0); // Starts from position 0 on the display
}


void check_button_press() {
    char keyPress[3] = {0};

    if (currentLevel == 0) {
    display.showNumberDec(currentLevel, true);
    }

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
                        currentLevel = 0;
                    }
                    while (digitalRead(ROW_PINS[row]) == LOW) {}
                }
            }
        }
        digitalWrite(COL_PINS[col], HIGH);
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


void blink_display(char num) {
    display.showNumberDec(num - '0', true);
    delay(500);
    display.clear();
    delay(500);
}
