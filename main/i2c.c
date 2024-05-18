#include "i2c.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

void init_i2c() {
    stdio_init_all();
	i2c_init(i2c_default, 100 * 1000);

	// Initialize I2C pins - sda(16), scl(17)
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

int read_i2c(uint8_t addr, uint8_t *output, size_t len) {
    // false - finished with bus
    return i2c_read_blocking (i2c_default, addr, output, len, false);
}

int write_i2c(uint8_t addr, uint8_t *input, size_t len) {
    // true to keep master control of bus
    return i2c_write_blocking (i2c_default, addr, input, len, true);
}
