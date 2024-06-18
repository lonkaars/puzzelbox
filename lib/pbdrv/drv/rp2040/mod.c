#include "mod.h"

#include "../../pb.h"
#include "../../pb-types.h"
#include "../../pb-mod.h"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <pico/i2c_slave.h>

#define PBDRV_I2C i2c0
#define BUF_SIZE 256

static volatile bool pbdrv_i2c_msg_avail = false;
static uint8_t data[BUF_SIZE];
static size_t size = 0;

// TODO: create event group instead of pbdrv_i2c_msg_avail
// TODO: create freertos task that monitors the pbdrv_i2c_msg_avail flag and
// calls pbdrv_i2c_recv when a message is received


/**
 * \note this function is called from the I2C ISR, and should return as quickly
 * as possible.
 */
static void recv_event(i2c_inst_t *i2c, i2c_slave_event_t event) {
	// message needs to be handled first
	if (pbdrv_i2c_msg_avail) return;

	switch (event) {
		case I2C_SLAVE_RECEIVE: {
			if (size == BUF_SIZE) return;
			data[size++] = i2c_read_byte_raw(PBDRV_I2C);
			break;
		}
		case I2C_SLAVE_FINISH: {
			// TODO: handle this w/ queue mechanism instead?
			// pbdrv_i2c_recv(data, size);
			pbdrv_i2c_msg_avail = true;
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

/**
 * While the RP2040's datasheet claims it supports multi-master configurations
 * by implementing bus arbitration, it does not natively support a mode where
 * it is configured as a (multi-)master with a slave address, such that it can
 * be addressed by other multi-masters. This function includes a hacky
 * workaround that teporarily sets the RP2040 to I2C master mode to send a
 * message, and then restores it back to slave mode.
 *
 * This approach results in some received frames being (partially) dropped in
 * the time period between the invocation of this function and the bus becoming
 * idle (and the message is sent).
 */
__weak void pbdrv_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	i2c_set_slave_mode(PBDRV_I2C, false, PBDRV_MOD_ADDR);

	// false to write stop condition to i2c bus
	i2c_write_timeout_us(PBDRV_I2C, addr, buf, sz, false, PB_TIMEOUT_US);

	i2c_set_slave_mode(PBDRV_I2C, true, PBDRV_MOD_ADDR);
}

