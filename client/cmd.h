#pragma once

#include <stddef.h>

typedef void cmd_fn_t(char *);

struct cmd {
	void (* handle)(char *);
	const char* name;
	const char* info;
	// TODO: tab completion function?
};

cmd_fn_t cmd_exit;
cmd_fn_t cmd_test;
cmd_fn_t cmd_help;
cmd_fn_t cmd_status;
cmd_fn_t cmd_reset;
cmd_fn_t cmd_ls;
cmd_fn_t cmd_send;

static const struct cmd cmds[] = {
	{
		.handle = cmd_exit,
		.name = "exit",
		.info = "exit pbc",
	},
	{
		.handle = cmd_test,
		.name = "test",
		.info = "send a test puzbus message",
	},
	{
		.handle = cmd_help,
		.name = "help",
		.info = "show this help",
	},
	{
		.handle = cmd_status,
		.name = "status",
		.info = "show global puzzle box state (main controller state)",
	},
	{
		.handle = cmd_reset,
		.name = "reset",
		.info = "reset entire game state",
	},
	{
		.handle = cmd_ls,
		.name = "ls",
		.info = "list connected puzzle modules",
	},
#ifdef DEBUG
	{
		.handle = cmd_send,
		.name = "send",
		.info = "[debug] send raw message",
	},
#endif
};
static const size_t cmds_length = sizeof(cmds) / sizeof(cmds[0]);

