#include "i2c.h"
#include "init.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

void init_i2c() {
	i2c_init(I2C_PORT, 100 * 1000); // currently at 100kHz

	// Initialize I2C pins - sda(16), scl(17)
	gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

	gpio_pull_up(SDA_PIN);
	gpio_pull_up(SCL_PIN);
}

int read_i2c(uint8_t addr, uint8_t *output, size_t len) {
	// false - finished with bus
	return i2c_read_blocking (I2C_PORT, addr, output, len, false);
}

int write_i2c(uint8_t addr, uint8_t *input, size_t len) {
	// true to keep master control of bus
	return i2c_write_blocking (I2C_PORT, addr, input, len, true);
}

bool reserved_addr(uint8_t addr) {
	return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void init_addr_array(uint8_t array[], int size) {
	for(int i = 0; i < size; i++){
		array[i] = 0x00;
	}
}

// Make sure that current addresses are checked (modules), and invalid addresses are ignore (neotrellis slave)
uint8_t* scan_bus(uint8_t *array) {
	int ret;
	int i = 0;
	uint8_t rxdata;

	for(int addr = 1; addr < (1<<7); addr++) {
		// ignore reserved addresses
		// These are any addresses of the form 000 0xxx or 111 1xxx
		
		// if( reserved_addr(addr) ){
		// 	ret = PICO_ERROR_GENERIC;
		// }else{
		// 	
		ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);
		//}

		// if acknowledged -> ret == number of bytes sent
		if(ret > 0){
			printf("found i2c slave on addr: %d\n", addr);
			array[i] = addr;
			i++;
		}
	}

	return array;
}

void bus_task() {
	// scan bus for slaves
	// send updates at regular intervals
	await_init();
	
	int i = 0;
	uint8_t found[MAX_SLAVES];
	init_addr_array(found, MAX_SLAVES);
	scan_bus(found);

	while(1) {
		// printf("Bus scan!");

		uint8_t data;
		for(int i = 0; i < MAX_SLAVES; i++){
			if( found[i] == 0x00 )
				continue;
			
			read_i2c(found[i], &data, 2);
			if(data > 0) {
				printf("Data: %d", data);
			}

		}
	}
}
