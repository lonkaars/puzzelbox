#include <iostream>
#include <string>
#include <array>

// Definitions for puzzle requirements
constexpr int TOTAL_LEVELS = 5;

// Enumeration for the states of the puzzle
enum PuzzleState {
    STATE_UNINITIALIZED,
    STATE_RESET,
    STATE_PLAYING,
    STATE_SOLVED,
    STATE_ERROR
};

// This array maps each level to the correct button press
const std::array<std::string, TOTAL_LEVELS> validButtons = {"A3", "F1", "U4", "C2", "L1"};

PuzzleState puzzleState = STATE_UNINITIALIZED;
int currentLevel = 0;

// Function prototypes
void displayCode(int level);
void sendI2CUpdate(PuzzleState state);

// Simulate sending an I2C update
void sendI2CUpdate(PuzzleState state) {
    std::cout << "Sending state " << state << " to main controller via I2C.\n";
}

// Simulate checking if the vault door is closed
bool isVaultClosed() {
    return true; // Return true if the door sensor indicates closed
}

// Function to display a code on the 7-segment display
void displayCode(int level) {
    std::cout << "Displaying code for level " << level << " on the 7-segment display.\n";
}

// Function to initialize the puzzle
void initializePuzzle() {
    if (isVaultClosed()) {
        puzzleState = STATE_RESET;
        currentLevel = 1;  // Start at level 1
        std::cout << "Puzzle initialized. Starting at level " << currentLevel << ".\n";
        displayCode(currentLevel); // Show the first code
    } else {
        std::cout << "Vault door is open. Please close the door to start the puzzle.\n";
    }
}

// Function to lock the vault
void lockVault() {
    std::cout << "Vault locked.\n";
}

// Function to unlock the vault
void unlockVault() {
    std::cout << "Vault unlocked!\n";
}

// Function to simulate the buzzer sound
void playErrorSound() {
    std::cout << "Playing error sound.\n";
}

// Function to simulate blinking the 7-segment display
void blinkDisplay() {
    std::cout << "7-segment display is blinking to indicate an error.\n";
}

// Validate the button press for the current level
bool isValidButtonPress(const std::string& button, int level) {
    return button == validButtons[level - 1];
}

// Function to update the state of the puzzle based on the current level
void updateStateAfterButtonPress(bool validPress) {
    if (validPress) {
        if (currentLevel >= TOTAL_LEVELS) {
            puzzleState = STATE_SOLVED;
            unlockVault();
        } else {
            puzzleState = STATE_PLAYING;
            displayCode(currentLevel);
        }
    } else {
        puzzleState = STATE_ERROR;
        playErrorSound();
        blinkDisplay();
        lockVault();
        currentLevel = 1;  // Reset to level 1
        displayCode(currentLevel);
    }
    sendI2CUpdate(puzzleState);  // Notify main controller of the state change
}

int main() {
    initializePuzzle();
    
    std::string buttonInput;

    while (puzzleState != STATE_SOLVED) {
        std::cout << "Enter the button pressed for level " << currentLevel << " (format Xn, e.g., A3): ";
        std::getline(std::cin, buttonInput);

        if (!buttonInput.empty() && isValidButtonPress(buttonInput, currentLevel)) {
            currentLevel++;
            if (currentLevel > TOTAL_LEVELS) {
                puzzleState = STATE_SOLVED;
                unlockVault();
                std::cout << "The puzzle is solved and the vault is open!\n";
            } else {
                displayCode(currentLevel);
            }
        } else {
            playErrorSound();
            blinkDisplay();
            lockVault();
            puzzleState = STATE_RESET;
            currentLevel = 1;
            displayCode(currentLevel);
        }
        sendI2CUpdate(puzzleState);
    }

    return 0;
}
