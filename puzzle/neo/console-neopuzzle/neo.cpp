#include <iostream>
#include <array>

#define MATRIX_SIZE 8

enum NeoState {
    NEO_UNINITIALIZED,
    NEO_PLAYING,
    NEO_SOLVED
};

// Simulate the 8x8 LED matrix with a 2D array
std::array<std::array<bool, MATRIX_SIZE>, MATRIX_SIZE> neoMatrix;

NeoState neoState = NEO_UNINITIALIZED;

// Helper function to toggle LEDs if within bounds
void toggleIfValid(int x, int y) {
    if (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE) {
        neoMatrix[x][y] = !neoMatrix[x][y];
    }
}

void initializeNeoMatrix() {
    // The initial pattern from the Appendix A example (assuming red is 'true'/on and white is 'false'/off)
    std::array<std::array<bool, MATRIX_SIZE>, MATRIX_SIZE> initialPattern = {{
        {false, true, false, true, false, true, false, true},
        {true, false, true, false, true, false, true, false},
        {false, true, false, true, false, true, false, true},
        {true, false, true, false, true, false, true, false},
        {false, true, false, true, false, true, false, true},
        {true, false, true, false, true, false, true, false},
        {false, true, false, true, false, true, false, true},
        {true, false, true, false, true, false, true, false}
    }};

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            neoMatrix[i][j] = initialPattern[i][j];
        }
    }

    neoState = NEO_PLAYING;
}


void printNeoMatrix() {
    // Print the matrix state to the console
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            std::cout << (neoMatrix[i][j] ? 1 : 0) << " ";
        }
        std::cout << std::endl;
    }
}

void toggleAdjacentLEDs(int x, int y) {
    // Toggle the LED at (x, y) and adjacent LEDs
    toggleIfValid(x, y);     // Center
    toggleIfValid(x - 1, y); // Up
    toggleIfValid(x + 1, y); // Down
    toggleIfValid(x, y - 1); // Left
    toggleIfValid(x, y + 1); // Right
}


bool isNeoPuzzleSolved() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (neoMatrix[i][j]) return false; // If any LED is on, puzzle is not solved
        }
    }
    return true;
}

/// Integration needed
int main() {
    initializeNeoMatrix();
    printNeoMatrix();

    while (neoState != NEO_SOLVED) {
        int x, y;
        std::cout << "Enter the coordinates of the button pressed (x y): ";
        std::cin >> x >> y;

        if (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE) {
            toggleAdjacentLEDs(x, y);
            printNeoMatrix();

            if (isNeoPuzzleSolved()) {
                neoState = NEO_SOLVED;
                std::cout << "The NeoTrellis puzzle is solved!\n";
            }
        } else {
            std::cout << "Invalid coordinates. Please enter values between 0 and " << MATRIX_SIZE - 1 << ".\n";
        }
    }

    return 0;
}
