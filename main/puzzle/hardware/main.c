#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
#define CORRECT_LOGIC_GATES_VALUE {1, 0, 1, 0, 1, 0, 1, 0}
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

// Puzzle state structure
typedef struct {
    PlayingSubState playing_substate;
    char code[MORSE_CODE_LENGTH + 1];   // Buffer for the generated code
    int logic_gate_values[8];           // Buffer for logic gate values
    int potentiometer_values[NUMBER_OF_POTENTIOMETERS];
    int correct_potentiometer_values[MORSE_CODE_LENGTH];
    bool tone_playing;
    int morse_code_step;
    int morse_code_number_step;
    unsigned long ms_delay;
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


int main() {
    srand(time(NULL)); 

    enterState(UNINITIALIZED);
    
    while (currentState != SOLVED) {
        enterState(currentState);
    }

    return 0;
}

void enterState(HardwarePuzzleState state) {
    currentState = state;

    switch (currentState) {
        case UNINITIALIZED:
            printf("System is uninitialized.\n");
            enterState(IDLE); 
            break;
        case IDLE:
            printf("System is idle. Starting puzzle...\n");
            enterState(PLAYING); 
            break;
        case PLAYING:
            printf("Playing the puzzle...\n");
            puzzleState.playing_substate = PLAYING_MORSECODE; 
            generate_random_four_digit_code();
            change_gamestage(IDLE); 
            break;
        case SOLVED:
            printf("Congratulations! Puzzle solved.\n");
            
            break;
    }
}

void generate_random_four_digit_code() {
    printf("Generating random Morse code...\nCode: ");
    for (int i = 0; i < MORSE_CODE_LENGTH; i++) {
        puzzleState.correct_potentiometer_values[i] = rand() % 10;
        puzzleState.code[i] = puzzleState.correct_potentiometer_values[i] + '0'; 
    }
    puzzleState.code[MORSE_CODE_LENGTH] = '\0'; 
    printf("%s\n", puzzleState.code);
}

// Render function based on the current substate
void render() {
    if (puzzleState.ms_delay == 0) {
        puzzleState.ms_delay = millis() + play_morse_code();
    } else if (millis() >= puzzleState.ms_delay) {
        puzzleState.ms_delay = 0;
    }

    if (puzzleState.playing_substate == PLAYING_MORSECODE) {
        int values = puzzleState.potentiometer_values[0] * 1000 +
                     puzzleState.potentiometer_values[1] * 100 +
                     puzzleState.potentiometer_values[2] * 10 +
                     puzzleState.potentiometer_values[3];
        // Set display value using GPIO manager
        // GpioManager::instance().set_display_value(values);
    }
}

// Function to handle staying in the current state
void on_stay() {
    switch (puzzleState.playing_substate) {
        case PLAYING_MORSECODE:
            render();
            break;
        case PLAYING_LOGICGATES:
            // Get user input for logic gates
                puzzle_check(); // Check puzzle status (uncomment when implemented)
            break;
    }
}

// Function to check puzzle status based on the current substate
void puzzle_check() {
    switch (puzzleState.playing_substate) {
        case PLAYING_LOGICGATES:
            for (int i = 0; i < 8; i++) {
                if (puzzleState.logic_gate_values[i] != CORRECT_LOGIC_GATES_VALUE[i]) {
                    return;
                }
            }
            puzzleState.playing_substate = PLAYING_MORSECODE;
            break;
        case PLAYING_MORSECODE:
                if (morse_code_input_correct()) {
                    currentState = SOLVED; // Puzzle solved, transition to SOLVED-state
                }
            break;
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
                    if (morse_signal == '.') {                             //korte toon
                        GpioManager::instance().set_buzzer_pin(440);
                        return SHORT_TONE_DURATION;
                    } else if (morse_signal == '-') {                      //lange toon
                        GpioManager::instance().set_buzzer_pin(440);
                        return LONG_TONE_DURATION;
                    }
                }
            } else {
                GpioManager::instance().set_buzzer_pin(0);
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

// Function to change the game stage (transition between states)
void change_gamestage(HardwarePuzzleState state) {
    currentState = state;
}
