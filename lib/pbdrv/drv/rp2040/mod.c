#include "mod.h"

#include "../../pb.h"
#include "../../pb-types.h"
#include "../../pb-mod.h"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <pico/i2c_slave.h>

#define PBDRV_I2C i2c0
#define BUF_SIZE 256

// NOTE: this function is called from the I2C ISR, and should return as quickly
// as possible.
static void recv_event(i2c_inst_t *i2c, i2c_slave_event_t event) {
	uint8_t data[BUF_SIZE];
	size_t size = 0;
	// pbdrv_i2c_recv(NULL, 0);

	switch (event) {
		case I2C_SLAVE_RECEIVE: {
			if (size == BUF_SIZE) return;
			data[size++] = i2c_read_byte_raw(PBDRV_I2C);
			break;
		}
		case I2C_SLAVE_FINISH: {
			// TODO: handle this w/ queue mechanism instead?
			pbdrv_i2c_recv(data, size);
			size = 0;
			break;
		}
		default: break;
	}
}

void pbdrv_setup() {
	i2c_init(PBDRV_I2C, PB_CLOCK_SPEED_HZ);
	i2c_slave_init(PBDRV_I2C, PBDRV_MOD_ADDR, &recv_event);
}

__weak void pbdrv_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	i2c_set_slave_mode(PBDRV_I2C, false, PBDRV_MOD_ADDR);

	// false to write stop condition to i2c bus
	i2c_write_timeout_us(PBDRV_I2C, addr, buf, sz, false, PB_TIMEOUT_US);

	i2c_set_slave_mode(PBDRV_I2C, true, PBDRV_MOD_ADDR);
}

