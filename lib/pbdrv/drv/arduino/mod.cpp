#ifndef ARDUINO
#error This driver only works on the Arduino platform!
#endif

#include <Arduino.h>
#include <Wire.h>
#include <avr/delay.h>

#include <FreeRTOS.h>
#include <timers.h>
#include <task.h>

#include <stdlib.h>
#include <stdint.h>

#include "../../pb.h"
#include "../../pb-mod.h"
#include "../../pb-buf.h"

static void async_pb_i2c_recv(void * _msg, uint32_t _) {
	pb_buf_t * msg = (pb_buf_t *) _msg;
	pb_i2c_recv((uint8_t *) msg->data, msg->size);
	pb_buf_free(msg);
	free(msg);
}

static void recv_event(int bytes) {
	pb_buf_t * msg = (pb_buf_t *) malloc(sizeof(pb_buf_t));
	msg->data = (char *) malloc(bytes);
	msg->size = 0;
	while (Wire.available())
		msg->data[msg->size++] = Wire.read();

	// defer pb_i2c_recv call
	xTimerPendFunctionCallFromISR(async_pb_i2c_recv, msg, 0, NULL);
}

static void pb_setup() {
	Wire.begin((int) PB_MOD_ADDR);
	Wire.setWireTimeout(PB_TIMEOUT_US, true);
	Wire.setClock(PB_CLOCK_SPEED_HZ);
	// TODO: check if onReceive replaces or appends a handler function
	Wire.onReceive(recv_event);
}

__weak void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz) {
	Wire.beginTransmission((int) addr);
	Wire.write(buf, sz);
	Wire.endTransmission(true);
	Wire.setWireTimeout(PB_TIMEOUT_US, true);
}

//! Arduino setup function
extern void setup(void);
//! Arduino loop function
extern void loop(void);
//! Arduino internal initialization
void init(void);

//! FreeRTOS loop task
void loop_task() {
	for(;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
}

//! Application entrypoint
int main(void) {
	init(); // call arduino internal setup
	setup(); // call regular arduino setup
	pb_setup(); // call pbdrv-mod setup
	xTaskCreate((TaskFunction_t) loop_task, "loop", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler(); // start freertos scheduler
	return 0;
}

/**
 * \note I should really be able to use Arduino's initVariant function for
 * this, but I can't seem to get it to link properly using the CMake setup in
 * this repository. Overriding the main() function seems to work, and the
 * USBCON thing in the default Arduino main() function isn't needed because
 * puzzle modules are likely not using USB.
 */

