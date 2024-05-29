#pragma once

#include <stddef.h>

typedef void cmd_handle_t(char *);
typedef char** cmd_complete_t(const char*, int, int);

struct cmd {
	cmd_handle_t * handle;
	const char* name;
	const char* info;
	cmd_complete_t * complete;
};
typedef struct cmd cmd_t;

cmd_handle_t cmd_exit;
cmd_handle_t cmd_test;
cmd_handle_t cmd_help;
cmd_complete_t cmd_help_complete;
cmd_handle_t cmd_reset;
cmd_handle_t cmd_ls;
cmd_handle_t cmd_send;
cmd_handle_t cmd_skip;

static const cmd_t cmds[] = {
	{
		.handle = cmd_exit,
		.name = "exit",
		.info = "exit pbc",
	},
	{
		.handle = cmd_help,
		.name = "help",
		.info = "show this help",
	},
	{
		.handle = cmd_reset,
		.name = "reset",
		.info = "set game state to 'idle' for one or more puzzle modules",
	},
	{
		.handle = cmd_skip,
		.name = "skip",
		.info = "set game state to 'solved' for one or more puzzle modules",
	},
	{
		.handle = cmd_ls,
		.name = "ls",
		.info = "list connected puzzle modules and their state",
	},
#ifdef DEBUG
	{
		.handle = cmd_send,
		.name = "send",
		.info = "[debug] send raw message",
	},
	{
		.handle = cmd_test,
		.name = "test",
		.info = "[debug] send a test puzbus message",
	},
#endif
};
static const size_t cmds_length = sizeof(cmds) / sizeof(cmds[0]);

