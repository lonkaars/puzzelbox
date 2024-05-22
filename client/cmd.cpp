#include <cstdlib>
#include <string.h>

#include "cmd.h"
#include "sock.h"

void cmd_exit(char*) {
	exit(EXIT_SUCCESS);
}

void cmd_test(char*) {
	const char* data = "Hello world!";
	i2c_send(0x39, (char*) data, strlen(data));
}

