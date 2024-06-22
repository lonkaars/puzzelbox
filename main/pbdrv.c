#include "pb.h"

#include "pb.h"
#include "pb-types.h"
#include "pb-mod.h"
#include "pb-send.h"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <pico/i2c_slave.h>
#include <pico/stdio.h>

#include <FreeRTOS.h>
#include <stdio.h>
#include <timers.h>

#define PB_I2C_S i2c0
#define PB_I2C_M i2c1

#define BUF_SIZE 256
#define MSGS_MAX 4

typedef struct {
	uint8_t data[BUF_SIZE];
	size_t size;
} i2c_msg_buf_t;

static i2c_msg_buf_t msgs[MSGS_MAX];
static size_t msg_index = 0;

static void async_pb_i2c_recv(void * _msg, uint32_t _) {
	i2c_msg_buf_t * msg = _msg;
	pb_i2c_recv(msg->data, msg->size);
}

static void msg_complete(i2c_msg_buf_t * msg) {
	return pb_i2c_recv(msg->data, msg->size);

	// defer pb_i2c_recv call to FreeRTOS scheduler as pb_i2c_recv takes
	// too long to return from an ISR
	xTimerPendFunctionCallFromISR(async_pb_i2c_recv, msg, 0, NULL);

	// prepare next message for use
	msg_index = (msg_index + 1) % MSGS_MAX;
	msgs[msg_index].size = 0;
}

// This function is called from the I2C ISR
static void recv_event(i2c_inst_t *i2c, i2c_slave_event_t event) {
	i2c_msg_buf_t * msg = &msgs[msg_index];

	switch (event) {
		case I2C_SLAVE_RECEIVE: {
			if (msg->size == BUF_SIZE) return;
			msg->data[msg->size++] = i2c_read_byte_raw(PB_I2C_S);
			break;
		}
		case I2C_SLAVE_FINISH: {
			msg_complete(msg);
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

void pb_mod_blocking_delay_ms(unsigned long ms) {
	vTaskDelay(ms / portTICK_PERIOD_MS);
}

