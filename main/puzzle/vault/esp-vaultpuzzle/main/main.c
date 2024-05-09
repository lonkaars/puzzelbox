#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c.h"

// Definitions for GPIO numbers, change these according to your hardware setup
#define BUTTON_GPIO GPIO_NUM_0 // Example GPIO for button input (not used directly in final implementation)
#define TOTAL_LEVELS 5
#define TAG "VaultPuzzle"
#define I2C_MASTER_NUM I2C_NUM_0

// Key Matrix Pin Configuration
#define ROWS 4
#define COLS 3
#define ROW_PINS {GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25, GPIO_NUM_26}
#define COL_PINS {GPIO_NUM_27, GPIO_NUM_14, GPIO_NUM_12}

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
void i2c_master_init();
void send_i2c_update(PuzzleState state);
void display_code(int level);
void initialize_system();
void check_button_press();

// Initialize I2C and GPIO for keypad and display
void i2c_master_init() {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
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
    ESP_LOGI(TAG, "Sending state %d to main controller via I2C.", state);
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "State update sent successfully.");
    } else {
        ESP_LOGE(TAG, "Failed to send state update via I2C.");
    }
}

void display_code(int level) {
    ESP_LOGI(TAG, "Displaying code for level %d on the 7-segment display.", level);
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, level, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Code for level %d displayed successfully.", level);
    } else {
        ESP_LOGE(TAG, "Failed to display code for level %d via I2C.", level);
    }
}

void initialize_system() {
    gpio_config_t io_conf;
    // Configure the rows as input with pull-up
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ROW_PINS[0]) | (1ULL << ROW_PINS[1]) | (1ULL << ROW_PINS[2]) | (1ULL << ROW_PINS[3]);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    
    // Configure the columns as output
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << COL_PINS[0]) | (1ULL << COL_PINS[1]) | (1ULL << COL_PINS[2]);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void check_button_press() {
    char keyPress[3] = {0};
    for (int col = 0; col < COLS; col++) {
        gpio_set_level(COL_PINS[col], 0); // Activate column
        for (int row = 0; row < ROWS; row++) {
            if (gpio_get_level(ROW_PINS[row]) == 0) { // Detect if any row is activated
                keyPress[0] = 'A' + row;
                keyPress[1] = '1' + col;
                keyPress[2] = '\0';
                ESP_LOGI(TAG, "Keypress detected: %s", keyPress);
                update_state_after_button_press(strcmp(keyPress, validButtons[currentLevel]) == 0);
                while (gpio_get_level(ROW_PINS[row]) == 0) {} // Wait for release
            }
        }
        gpio_set_level(COL_PINS[col], 1); // Deactivate column
    }
}

void update_state_after_button_press(bool validPress) {
    if (validPress) {
        if (currentLevel >= TOTAL_LEVELS) {
            puzzleState = STATE_SOLVED;
            ESP_LOGI(TAG, "Puzzle solved!");
            send_i2c_update(puzzleState);
        } else {
            puzzleState = STATE_PLAYING;
            currentLevel++;
            display_code(currentLevel);
        }
    } else {
        puzzleState = STATE_ERROR;
        ESP_LOGE(TAG, "Error in input");
    }
    send_i2c_update(puzzleState);
}

void app_main() {
    i2c_master_init();
    initialize_system();
    ESP_LOGI("App", "GPIO and I2C initialized.");
    
    while (puzzleState != STATE_SOLVED) {
        check_button_press();
        vTaskDelay(pdMS_TO_TICKS(100)); // Non-blocking delay
    }
}
