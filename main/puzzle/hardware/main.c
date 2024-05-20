#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Arduino.h> // Voor millis()
#include <string.h>  // Voor de strlen functie

typedef enum {
    UNINITIALIZED,
    IDLE,
    PLAYING,
    SOLVED
} HardwarePuzzleState;

typedef enum {
    PLAYING_MORSECODE,
    PLAYING_LOGICGATES
} PlayingSubState;

#define MORSE_CODE_LENGTH 4
#define NUMBER_OF_POTENTIOMETERS 4
const int CORRECT_LOGIC_GATES_VALUE[8] = {0, 1, 1, 0, 0, 0, 1, 0};
#define LONG_TONE_DURATION 1000   // Duur van een lange toon (1 seconde)
#define SHORT_TONE_DURATION 500   // Duur van een korte toon (0.5 seconde)
#define SHORT_PAUSE_DURATION 500  // Duur van een korte pauze na een toon (0.5 seconde)
#define LONG_PAUSE_DURATION 1000  // Duur van een lange pauze na een letter (1 seconde)

const char* MORSE_CODE[10] = {
    ".----",   // 1
    "..---",   // 2
    "...--",   // 3
    "....-",   // 4
    ".....",   // 5
    "-....",   // 6
    "--...",   // 7
    "---..",   // 8
    "----.",   // 9
    "-----"    // 0
};

HardwarePuzzleState currentState = UNINITIALIZED;

typedef struct {
    PlayingSubState playing_substate;
    char code[MORSE_CODE_LENGTH + 1];
    int logic_gate_values[8];
    int potentiometer_values[NUMBER_OF_POTENTIOMETERS];
    int correct_potentiometer_values[MORSE_CODE_LENGTH];
    bool tone_playing;
    int morse_code_step;
    int morse_code_number_step;
    unsigned long ms_delay;
    char user_morse_code[MORSE_CODE_LENGTH + 1];
} PuzzleState;

PuzzleState puzzleState;

// Function prototypes
void enterState(HardwarePuzzleState state);
void generate_random_four_digit_code();
void render();
void on_stay();
void puzzle_check();
void change_gamestage(HardwarePuzzleState state);
int play_morse_code();
bool morse_code_input_correct();
bool check_logic_gates();
void read_serial_input();
void read_morse_code_input();

void setup() {
    Serial.begin(9600);

    // Willekeurigheidsinitialisatie met een variabele waarde
    unsigned long seed = analogRead(A0) * analogRead(A1);
    randomSeed(seed);

    enterState(UNINITIALIZED);
}

void loop() {
    if (currentState != SOLVED) {
        on_stay();
    }
}

void enterState(HardwarePuzzleState state) {
    currentState = state;

    switch (currentState) {
        case UNINITIALIZED:
            Serial.println("System is uninitialized.");
            enterState(IDLE);
            break;
        case IDLE:
            Serial.println("System is idle. Starting puzzle...");
            enterState(PLAYING);
            break;
        case PLAYING:
            Serial.println("Playing the puzzle...");
            puzzleState.playing_substate = PLAYING_LOGICGATES; // Start with logic gates
            Serial.println("Enter 8 logic gate values (0 or 1) separated by spaces:");
            break;
        case SOLVED:
            Serial.println("Congratulations! Puzzle solved.");
            break;
    }
}

void generate_random_four_digit_code() {
    Serial.print("Generating random Morse code...\nCode: ");
    for (int i = 0; i < MORSE_CODE_LENGTH; i++) {
        int random_index = random(0, 10); // Genereer een willekeurig getal tussen 0 en 9
        puzzleState.correct_potentiometer_values[i] = random_index;
        puzzleState.code[i] = random_index + '0'; // Zet het getal om naar een karakter
    }
    puzzleState.code[MORSE_CODE_LENGTH] = '\0'; 
    Serial.println(puzzleState.code);
}

void render() {
    if (puzzleState.ms_delay == 0) {
        puzzleState.ms_delay = millis() + play_morse_code();
    } else if (millis() >= puzzleState.ms_delay) {
        puzzleState.ms_delay = 0;
    }
}

void on_stay() {
    switch (currentState) {
        case PLAYING:
            if (puzzleState.playing_substate == PLAYING_LOGICGATES) {
                read_serial_input(); // Read input from serial monitor
                if (check_logic_gates()) {
                    puzzleState.playing_substate = PLAYING_MORSECODE;
                    Serial.println("Logic gates correct. Proceeding to Morse code.");
                    generate_random_four_digit_code(); // Generate Morse code after logic gates are correct
                    Serial.println("Enter the Morse code as a 4-digit number:");
                }
            } else if (puzzleState.playing_substate == PLAYING_MORSECODE) {
                read_morse_code_input(); // Read Morse code input from serial monitor
                render();
            }
            break;
        default:
            break;
    }
}

void puzzle_check() {
    switch (puzzleState.playing_substate) {
        case PLAYING_LOGICGATES:
            if (check_logic_gates()) {
                puzzleState.playing_substate = PLAYING_MORSECODE;
            }
            break;
        case PLAYING_MORSECODE:
            if (morse_code_input_correct()) {
                currentState = SOLVED; // Puzzle solved, transition to SOLVED-state
            }
            break;
    }
}

bool morse_code_input_correct() {
    // Compare user input with generated code
    for (int i = 0; i < MORSE_CODE_LENGTH; i++) {
        if (puzzleState.user_morse_code[i] != puzzleState.code[i]) {
            return false;
        }
    }
    return true;
}

bool check_logic_gates() {
    for (int i = 0; i < 8; i++) {
        if (puzzleState.logic_gate_values[i] != CORRECT_LOGIC_GATES_VALUE[i]) {
            return false;
        }
    }
    return true;
}

void read_serial_input() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        int index = 0;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] == '0' || input[i] == '1') {
                if (index < 8) {
                    puzzleState.logic_gate_values[index] = input[i] - '0';
                    index++;
                }
            }
        }
        if (index == 8) {
            Serial.println("Logic gate values incorrect");
            for (int i = 0; i < 8; i++) {
                Serial.print(puzzleState.logic_gate_values[i]);
                Serial.print(" ");
            }
            Serial.println();
        } else {
            Serial.println("Invalid input. Please enter 8 logic gate values (0 or 1) separated by spaces:");
        }
    }
}

void read_morse_code_input() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        if (input.length() == MORSE_CODE_LENGTH) {
            input.toCharArray(puzzleState.user_morse_code, MORSE_CODE_LENGTH + 1);
            Serial.print("Morse code input received: ");
            Serial.println(puzzleState.user_morse_code);
            if (morse_code_input_correct()) {
                Serial.println("Morse code correct! Puzzle solved.");
                currentState = SOLVED;
            } else {
                Serial.println("Morse code incorrect. Try again.");
            }
        } else {
            Serial.println("Invalid input. Please enter the Morse code as a 4-digit number:");
        }
    }
}

int play_morse_code() {
    if (puzzleState.morse_code_step < MORSE_CODE_LENGTH) {
        if (puzzleState.morse_code_number_step < strlen(MORSE_CODE[0])) {
            if (!puzzleState.tone_playing) {
                puzzleState.tone_playing = true;
                char current_symbol = puzzleState.code[puzzleState.morse_code_step];
                if (current_symbol >= '0' && current_symbol <= '9') {
                    int morse_index = current_symbol - '0';
                    char morse_signal = MORSE_CODE[morse_index][puzzleState.morse_code_number_step];
                    if (morse_signal == '.') {
                        // GpioManager::instance().set_buzzer_pin(440);
                        //Serial.println("Playing short tone");
                        return SHORT_TONE_DURATION;
                    } else if (morse_signal == '-') {
                        // GpioManager::instance().set_buzzer_pin(440);
                        //Serial.println("Playing long tone");
                        return LONG_TONE_DURATION;
                    }
                }
            } else {
                // GpioManager::instance().set_buzzer_pin(0);
                //Serial.println("Pausing tone");
                puzzleState.tone_playing = false;
                puzzleState.morse_code_number_step++;
                return SHORT_PAUSE_DURATION;
            }
        } else {
            puzzleState.morse_code_number_step = 0;
            puzzleState.morse_code_step++;
            return LONG_PAUSE_DURATION;
        }
    } else {
        puzzleState.morse_code_step = 0;
        return LONG_PAUSE_DURATION;
    }
    return 0;
}

void change_gamestage(HardwarePuzzleState state) {
    currentState = state;
}