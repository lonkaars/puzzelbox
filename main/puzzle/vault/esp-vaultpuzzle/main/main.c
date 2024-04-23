#include <stdio.h>


#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c.h"

// Definitions for GPIO numbers, change these according to your hardware setup
#define BUTTON_GPIO GPIO_NUM_0 // Example GPIO for button input
#define TOTAL_LEVELS 5
#define TAG "VaultPuzzle"

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

void update_state_after_button_press(bool validPress);

void initialize_system();

void check_button_press();

void app_main();


void initialize_system() {
    gpio_config_t io_conf;
    // Disable interrupts
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // Set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    // Bit mask of the pins that you want to set, e.g., GPIO0
    io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO);
    // Disable pull-down mode
    io_conf.pull_down_en = 0;
    // Enable pull-up mode
    io_conf.pull_up_en = 1;
    // Configure GPIO with the given settings
    gpio_config(&io_conf);
}

void check_button_press() {
    int btn_state = gpio_get_level(BUTTON_GPIO);
    if (btn_state == 0) { // Assuming active low button
        ESP_LOGI(TAG, "Button pressed!");
        // Implement button press handling logic here
        update_state_after_button_press(true); // Example call
    }
}

void update_state_after_button_press(bool validPress) {
    if (validPress) {
        if (currentLevel >= TOTAL_LEVELS) {
            puzzleState = STATE_SOLVED;
            ESP_LOGI(TAG, "Puzzle solved!");
            // Add actions to perform on solve (e.g., unlock something)
        } else {
            puzzleState = STATE_PLAYING;
            currentLevel++;
            ESP_LOGI(TAG, "Advance to level %d", currentLevel);
        }
    } else {
        puzzleState = STATE_ERROR;
        ESP_LOGE(TAG, "Error in input");
        // Handle error state (e.g., reset puzzle or lock down system)
    }
}

void app_main() {
    initialize_system();
    ESP_LOGI("App", "GPIO %d initialized as input.", BUTTON_GPIO);

    while (puzzleState != STATE_SOLVED) {
        check_button_press();
        // Additional loop logic (delays or non-blocking checks)
        vTaskDelay(pdMS_TO_TICKS(100)); // Non-blocking delay
    }
}
