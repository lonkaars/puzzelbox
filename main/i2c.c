#include "i2c.h"
#include "init.h"
#include "sock.h"
#include "pb/types.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <string.h>

uint8_t found[MAX_SLAVES];
extern struct netconn* current_connection;

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

int write_read_i2c(uint8_t addr, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len){
	// herhaalde start conditie voor direct lezen na i2c write (?)
	int ret = write_i2c(addr, input, input_len);
	if (ret < 0) {
		printf("Write failure while writing data to bus.\n");
		return ret;
	}

	// wait for response
	absolute_time_t start_time = get_absolute_time();
	while ( absolute_time_diff_us(start_time, get_absolute_time()) / 1000 < MAX_TIMEOUT_TIME ){
		ret = read_i2c(addr, output, output_len);
		if( ret > 0 ) {
			return ret;
		}
		sleep_ms(1);
	}

	printf("Timeout occurred while waiting for slave response.\n");
	return -1;
}

// Make sure that current addresses are checked (modules), and invalid addresses are ignore (neotrellis slave)
uint8_t* scan_bus(uint8_t *array) {
	int ret;
	int i = 0;
	uint8_t * rxdata;
	uint8_t * handshake_data;
	init_addr_array(array, MAX_SLAVES);

	for(int addr = 1; addr < (1<<7); addr++) {	
		// fix handshake
		ret = read_i2c(addr, rxdata, 1);

		if ( ret <= 0 )
			continue;

		char buf[80];
		size_t s = snprintf(buf, 80,"found i2c puzzle module at address: 0x%02x\n", addr);
		netconn_write(current_connection, buf, s, NETCONN_COPY);
		printf("%.*s", s, buf);
			
		// do handshake
		ret = write_read_i2c(addr, (uint8_t*)pb_magic_msg, sizeof(pb_magic_msg), handshake_data, sizeof(pb_magic_res)); // fix data + length + everything	
		
		if ( ret != sizeof(pb_magic_res))
			continue;
		
		if ( ret > 0 && (memcmp(handshake_data, pb_magic_res, sizeof(pb_magic_res)) == 0)) {
			printf("this was an actual device!!!1111!\n");
			char buf[80];
			size_t s = snprintf(buf, 80,"found i2c puzzle module at address: 0x%02x\n");
			netconn_write(current_connection, buf, s, NETCONN_COPY);

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
	
	scan_bus(found);

	while(1) {
		// add check if bus is in use

		uint8_t data;
		for(int i = 0; i < MAX_SLAVES; i++){
			if( found[i] == 0x00 )
				continue;
			
			read_i2c(found[i], &data, 2);
			if(data > 0) {
				printf("Data: %d", data);
			}

		}

		sleep_ms(1000); // wait for one second before next loop
	}
}
