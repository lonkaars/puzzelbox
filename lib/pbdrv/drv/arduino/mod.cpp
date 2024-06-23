#ifndef ARDUINO
#error This driver only works on the Arduino platform!
#endif

#include <Arduino.h>
#include <Wire.h>
#include <avr/delay.h>

#include "../../pb.h"
#include "../../pb-mod.h"
#include "../../pb-types.h"

static void recv_event(int bytes) {
	uint8_t data[bytes];
	size_t sz = 0;
	while (Wire.available())
		data[sz++] = Wire.read();

	pb_i2c_recv(data, sz);
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

/**
 * \ingroup pb_drv_arduino
 * \brief Application entrypoint
 *
 * This function overrides the default (weak) implementation of the \c main()
 * function in the Arduino framework. No additional setup is required to use
 * this driver.
 *
 * \note I should really be able to use Arduino's initVariant function for
 * this, but I can't seem to get it to link properly using the CMake setup in
 * this repository. Overriding the main() function seems to work, and the
 * USBCON thing in the default Arduino main() function isn't needed because
 * puzzle modules are likely not using USB.
 */
int main(void) {
	init(); // call arduino internal setup
	setup(); // call regular arduino setup
	pb_setup(); // call pbdrv-mod setup
	for(;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
	return 0;
}

