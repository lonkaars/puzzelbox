#include "i2c.h"
#include "init.h"

#include "pb-mod.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

uint8_t* scan_bus(uint8_t *array) {
	int ret;
	int i = 0;
	uint8_t rxdata;

	for(int addr = 0; addr < (1<<7); addr++) {
		// ignore reserved addresses
		// These are any addresses of the form 000 0xxx or 111 1xxx
		// ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);

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

	vTaskDelay(1000 / portTICK_PERIOD_MS);
	
	// int i = 0;
	// uint8_t found[MAX_SLAVES];
	// init_addr_array(found, MAX_SLAVES);
	
	while (true) {
		vTaskDelay(9 / portTICK_PERIOD_MS);
		pbdrv_i2c_send(0x69, (uint8_t *) "bbbbbbbb", 9);

		// i2c_write_blocking(i2c0, 0x69, (uint8_t *) "bbbbbbbb", 9, false);
		// pbdrv_i2c_recv(NULL, 0);
	}

	// while(1) {
	// 	// printf("Bus scan!");
	// 	scan_bus(found);

	// 	for(int i = 0; i < MAX_SLAVES; i++){
	// 		if( found[i] == 0x00 )
	// 			break;
	// 		
	// 		uint8_t data = 0x01;
	// 		// send data to found slave address
	// 		write_i2c(found[i], &data, 1);

	// 		data = 0x02;
	// 		write_i2c(found[i], &data, 1);
	// 		// request update from slave addr at found[i]
	// 		//write_i2c();
	// 	}
	// }
}

