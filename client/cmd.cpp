#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>
#include <string.h>

#include "cmd.h"
#include "i2ctcpv1.h"
#include "rl.h"
#include "sock.h"
#include "parse.h"

#include "../shared/puzbus.h"

char* consume_token(char* input, const char* ifs) {
	strtok(input, ifs);
	return strtok(NULL, "\0");
}

void cmd_exit(char*) {
	exit(EXIT_SUCCESS);
}

void cmd_test(char*) {
	const char* data = "Hello world!";
	i2c_send(0x39, data, strlen(data));
}

void cmd_help(char*) {
	printf("List of available commands:\n");
	for (size_t i = 0; i < cmds_length; i++) {
		struct cmd cmd = cmds[i];
		printf("  %-*s", 10, cmd.name);
		if (cmd.info != NULL)
			printf("  %s", cmd.info);
		printf("\n");
	}

	printf(
		"\n"
		"See man pbc(1) for more info about specific commands\n"
		"Hint: you can use the TAB key to autocomplete commands\n"
	);
}

void cmd_send(char* addr_str) {
	char* data_str = consume_token(addr_str, IFS);

	char* end;
	uint16_t addr = strtol(addr_str, &end, 0);
	if (addr_str + strlen(addr_str) != end) {
		printf("address format error\n");
		return;
	}

	char* data;
	size_t data_size;
	int err = strtodata(data_str, &data, &data_size);
	if (err <= 0) {
		printf("data format error at index %d:\n%s\n%*s^\n",
				-err, data_str, -err, "");
		return;
	}

	printf("sending char data[%lu = 0x%02lx] to 0x%02x\n", data_size, data_size, addr);
	i2c_send(addr, data, data_size);

	free(data);
}

void cmd_status(char*) {
	const char msg[] = {
		PB_CMD_READ,
		0x00, // addr 0 = global state
	};
	i2c_send(BUSADDR_MAIN, msg, sizeof(msg));
	// NOTE: the reply handler will automatically print the state once it's
	// received
}

void cmd_reset(char*) {
	const char msg[] = {
		PB_CMD_WRITE,
		0x00,
		PB_GS_IDLE,
	};
	i2c_send(BUSADDR_MAIN, msg, sizeof(msg));
}

void cmd_skip(char*) {
	const char msg[] = {
		PB_CMD_WRITE,
		0x00,
		PB_GS_SOLVED,
	};
	i2c_send(BUSADDR_MAIN, msg, sizeof(msg));
}

void cmd_ls(char*) {
	return;
	const char msg[] = {
		PB_CMD_READ,
		// TODO: which address is this?
	};
	i2c_send(BUSADDR_MAIN, msg, sizeof(msg));
}

