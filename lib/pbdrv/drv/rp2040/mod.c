#include "pb.h"

#include "pb.h"
#include "pb-types.h"
#include "pb-mod.h"
#include "pb-send.h"
#include "pb-buf.h"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <pico/i2c_slave.h>

#define PB_I2C_S i2c0
#define PB_I2C_M i2c1

#define BUF_SIZE 256

uint8_t i2c_msg_buf[BUF_SIZE];
size_t i2c_msg_buf_sz = 0;

// This function is called from the I2C ISR
static void recv_event(i2c_inst_t *i2c, i2c_slave_event_t event) {
	switch (event) {
		case I2C_SLAVE_RECEIVE: {
			if (i2c_msg_buf_sz == BUF_SIZE) return;
			i2c_msg_buf[i2c_msg_buf_sz++] = i2c_read_byte_raw(PB_I2C_S);
			break;
		}
		case I2C_SLAVE_FINISH: {
			pb_i2c_recv(i2c_msg_buf, i2c_msg_buf_sz);
			i2c_msg_buf_sz = 0;
			break;
		}
		default: break;
	}
}

void pb_setup() {
	i2c_init(PB_I2C_S, PB_CLOCK_SPEED_HZ);
	i2c_init(PB_I2C_M, PB_CLOCK_SPEED_HZ);

	i2c_slave_init(PB_I2C_S, PB_MOD_ADDR, &recv_event);
}

__weak void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	// false to write stop condition to i2c bus
	i2c_write_timeout_us(PB_I2C_M, addr, buf, sz, false, PB_TIMEOUT_US);
}

