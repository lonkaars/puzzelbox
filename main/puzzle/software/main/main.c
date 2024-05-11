#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define TAG "SoftwarePuzzle"
#define I2C_MASTER_NUM I2C_NUM_0

// pins are sorted [up -> down] when looking at the puzzelbox;
#define TOTAL_VALID 5
#define INPUT_PINS {}
#define OUTPUT_PINS {}

typedef enum {
    UNINITIALIZED = 0U,
    RESET,
    PLAYING,
    SOLVED,
    ERROR
} PuzzleState;

PuzzleState state = UNINITIALIZED;
const char* validCombinations[TOTAL_VALID] = {"11", "22", "33", "44", "55"};

void I2CMasterInit() {
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = I2C_MASTER_SDA_IO;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = I2C_MASTER_SCL_IO;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, config.mode, 0, 0, 0);
}

void SendI2CUpdate(PuzzleState state) {
    uint8_t data;
    switch (state) {
        case UNINITIALIZED: data = 0x00; break;
        case RESET: data = 0x01; break;
        case PLAYING: data = 0x02; break;
        case SOLVED: data = 0x03; break;
        case ERROR: data = 0x04; break;
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

void SystemInit() {
    gpio_config_t config;

    // Configure the input row with pull-up
    config.intr_type = GPIO_INTR_DISABLE;
    config.mode = GPIO_MODE_INPUT;
    config.pin_bit_mask = (1ULL << INPUT_PINS[0]) | (1ULL << INPUT_PINS[1]) | (1ULL << INPUT_PINS[2]) | (1ULL << INPUT_PINS[3]);
    config.pull_down_en = 0;
    config.pull_up_en = 1;
    gpio_config(&config);

    // configure the output row
    config.mode = GPIO_MODE_OUTPUT;
    config.pin_bit_mask = (1ULL << OUTPUT_PINS[0]) | (1ULL << OUTPUT_PINS[1]) | (1ULL << OUTPUT_PINS[2]);
    config.pull_down_en = 0;
    config.pull_up_en = 0;
    gpio_config(&config);
}

bool ValidateCombination(const int in, const int out) {
    for ( int ans = 0; ans < TOTAL_VALID; ans++) {
        if ( validCombinations[ans][0] == in ) {
            if ( validCombinations[ans][1] == out )
                return true;
        }
    }
    return false;
}

void Update() {
    bool validCombination = true;

    for ( int outputPin = 0; outputPin < VALID_TOTAL; outputPin++ ){
        // if a wrong in/out combination was given stop checking
        if (validCombination == false) {
            return;
        }

        // set output pin LOW
        gpio_set_level(OUTPUT_PINS[outputPin], 0); 
        for ( int inputPin = 0; inputPin < VALID_TOTAL; intputPin++ ){
            // check which input is set to LOW
            if (gpio_get_level(INPUT_PINS[inputPin]) == 0) {
                // remove message when going into production
                ESP_LOGI(TAG, "Detected connection between output pin: '%s' and input pin: '%s'", outputPin, inputPin);
                // check if found connection is valid
                validCombination = ValidateCombination(inputPin, outputPin);
            }
        }

        // set output pin back to HIGH
        gpio_set_level(OUTPUT_PINS[outputPin], 1); 
    }

    if ( validCombination == true ) {
        ESP_LOGI(TAG, "Solved!");
        send_i2c_update(SOLVED);
        state = SOLVED;
    } 
}

void app_main()
{
    I2CMasterInit();
    SystemInit();
    ESP_LOGI("App", "GPIO and I2C initialized.");
    
    while (state != SOLVED) {
        if( state == ERROR ) {
            ESP_LOGE(TAG, "Error: Entered unknown state!");
            break;
        }

        // add check if puzzle is active
        // add request to get puzzle status from main framework

        Update();
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay to make sure it doesn't spam I2C
    }
}