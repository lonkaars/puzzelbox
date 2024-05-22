#pragma once

#include <stddef.h>

typedef void cmd_fn_t(char *);

struct cmd {
	const char* name;
	void (* handle)(char *);
	const char* info;
	// TODO: tab completion function?
};

cmd_fn_t cmd_exit;
cmd_fn_t cmd_test;

static const struct cmd cmds[] = {
	(struct cmd){ .name = "exit", .handle = cmd_exit, .info = NULL, },
	(struct cmd){ .name = "test", .handle = cmd_test, .info = NULL, },
};

static const size_t cmds_length = sizeof(cmds) / sizeof(cmds[0]);

