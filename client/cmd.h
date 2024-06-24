#pragma once

/**
 * \ingroup pbc
 * \defgroup pbc_cmd Commands
 * \brief Commands within \ref pbc
 *
 * \note A manpage is available containing end-user usage instructions inside
 * the \ref client folder in the source code repository. This page contains the
 * internal code documentation for the commands defined in \c pbc.
 *
 * \{
 */

#include <stddef.h>

/**
 * \internal
 * \brief Command handler function
 *
 * \param line Remaining text after command name on command line
 */
typedef void cmd_handle_t(char * line);
/**
 * \internal
 * \brief Command completion function
 *
 * \param text Current word to complete
 * \param start Index in \c rl_line_buffer of cursor position
 * \param end End index of \p text in \c rl_line_buffer
 *
 * \return Array of \c char* with suggestions. The array is terminated by a
 * NULL pointer.
 */
typedef char** cmd_complete_t(const char* text, int start, int end);

/**
 * \internal
 * \brief Command definition struct
 */
typedef struct {
	cmd_handle_t * handle; //!< Handler function (required)
	const char* name; //!< Command name (required)
	const char* info; //!< Command info (shown in help command) (optional = NULL)
	cmd_complete_t * complete; //!< Completion function (optional = NULL)
} cmd_t;

cmd_handle_t cmd_exit;
cmd_handle_t cmd_test;
cmd_handle_t cmd_help;
cmd_handle_t cmd_reset;
cmd_handle_t cmd_ls;
cmd_handle_t cmd_send;
cmd_handle_t cmd_skip;
cmd_handle_t cmd_dump;
cmd_complete_t cmd_dump_complete;

//! Commands
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
	{
		.handle = cmd_dump,
		.name = "dump",
		.info = "[debug] dump sent or received messages",
		.complete = cmd_dump_complete,
	},
#endif
};

//! Number of commands defined in \c cmds
static const size_t cmds_length = sizeof(cmds) / sizeof(cmds[0]);

/// \}

