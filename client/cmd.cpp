#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "cmd.h"
#include "sock.h"
#include "parse.h"

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
		"You can also use the TAB key to autocomplete commands\n"
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
	if (strtodata(data_str, &data, &data_size)) {
		printf("data format error at index %d:\n%s\n%*s^\n",
				(int) data_size, data_str, (int) data_size, "");
		return;
	}

	// printf("(0x%02x) -> \"%.*s\"\n", addr, data_size, data);
	i2c_send(addr, data, data_size);

	free(data);
}

