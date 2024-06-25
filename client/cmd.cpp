#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>
#include <string.h>

#include "cmd.h"
#include "pb-types.h"
#include "pb-buf.h"
#include "pb-send.h"
#include "rl.h"
#include "i2c.h"
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
		cmd_t cmd = cmds[i];
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

void cmd_send(char * addr_str) {
	if (addr_str == NULL) {
		printf("error: no address\n");
		return;
	}

	char* data_str = consume_token(addr_str, IFS);

	if (data_str == NULL) {
		printf("error: no data\n");
		return;
	}

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

static void cmd_set_state(char * line, pb_global_state_t state) {
	if (line == NULL) {
		printf("error: no address\n");
		return;
	}
	
	i2c_addr_t addr = strtol(line, NULL, 0);

	pb_buf_t buf = pb_send_state_set(state);
	i2c_send(addr, buf.data, buf.size);
	pb_buf_free(&buf);
}

void cmd_reset(char * line) {
	cmd_set_state(line, PB_GS_IDLE);
}

void cmd_skip(char * line) {
	cmd_set_state(line, PB_GS_SOLVED);
}

extern bool i2c_dump_send;
extern bool i2c_dump_recv;
const char * dump_modes[] = {
	"none",
	"send",
	"recv",
	"both",
	NULL,
};
void cmd_dump(char * mode) {
	consume_token(mode, IFS);
	mode += strspn(mode, IFS);

	for (int i = 0; dump_modes[i] != NULL; i++) {
		if (strcmp(mode, dump_modes[i]) == 0) {
			i2c_dump_send = (i >> 0) & 1;
			i2c_dump_recv = (i >> 1) & 1;
			return;
		}
	}

	printf("mode \"%s\" unknown\n", mode);
}
char** cmd_dump_complete(const char * text, int begin, int end) {
	int word = rl_word(rl_line_buffer, begin);
	if (word == 1) return rl_complete_list(text, dump_modes);
	return NULL;
}
