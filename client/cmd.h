#pragma once

/**
 * \ingroup pbc
 * \defgroup pbc_cmd Commands
 * \brief Commands within \ref pbc
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
typedef char ** cmd_complete_t(const char * text, int start, int end);

/**
 * \internal
 * \brief Command definition struct
 */
typedef struct {
	cmd_handle_t * handle; //!< Handler function (required)
	const char * name; //!< Command name (required)
	const char *
		info; //!< Command info (shown in help command) (optional = NULL)
	cmd_complete_t * complete; //!< Completion function (optional = NULL)
} cmd_t;

/**
 * \anchor pbc_cmd_usage
 * \name Command usage
 * \{
 */

/**
 * \brief \c exit command
 *
 * ```
 * (pbc) exit
 * ```
 *
 * Disconnect from the puzzle box and exit \c pbc. This command takes no
 * arguments.
 */
cmd_handle_t cmd_exit;
/**
 * \brief \c test command
 *
 * \note This command is only available in builds of \c pbc compiled with the
 * \c DEBUG flag
 *
 * ```
 * (pbc) test
 * ```
 * 
 * Send a test command containing the ASCII string "Hello world!" to I2C
 * address 0x39. This command takes no arguments.
 */
cmd_handle_t cmd_test;
/**
 * \brief \c help command
 *
 * ```
 * (pbc) help
 * ```
 *
 * Print a list of available commands with descriptions. This command takes no
 * arguments.
 */
cmd_handle_t cmd_help;
/**
 * \brief \c reset command
 *
 * ```
 * (pbc) reset <mod>
 * ```
 * 
 * Set a specific puzzle module's global state to *idle*. *mod* is the I2C
 * address of the puzzle module to reset. This parameter may be specified in
 * hexadecimal using a *0x* prefix or decimal.
 *
 */
cmd_handle_t cmd_reset;
/**
 * \brief \c send command
 *
 * \note This command is only available in builds of \c pbc compiled with the
 * \c DEBUG flag
 *
 * ```
 * (pbc) send <addr> <data>
 * ```
 *
 * Send arbitrary data specified by *data* to the I2C address specified by
 * *addr*. *data* may consist of multiple arguments separated by \ref IFS, in
 * which case the arguments are concatenated.
 *
 * The main controller will initiate an I2C write command if the address
 * specified in *addr* does not match that of the main controller. If the
 * addresses do match, the main controller interprets the message as if it were
 * being addressed by another I2C controller.
 *
 * \par Example
 * ```
 *            ADDRESS  DATA
 *            v~~~     v~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * (pbc) send 0x39     68:65:6c:6c:6f  44      0x20    'world'    33
 *                     ^~~~~~~~~~~~~~  ^~      ^~~~    ^~~~~~~    ^~
 *                     HEXSTR          NUMBER  NUMBER  STRING     NUMBER
 *                     (binary)        (dec)   (hex)   (literal)  (dec)
 * ```
 *
 * \par Datatypes
 * \parblock
 * - **NUMBER**
 *
 *   Numbers can be specified as decimal or hexadecimal using a "0x" prefix.
 *   All numbers are unsigned. Decimal literals are always cast to 8-bit
 *   integers, while hexadecimal literals are cast to the smallest type that
 *   will fit the specified number. Numbers are always sent as little endian.
 *   
 *   Examples: `0` `123` `255` `0x10` `0x1245` `0xdeadBEEF`
 *
 * - **HEXSTR**
 *
 *   Hexadecimal string literals are specified by hexadecimal bytes separated
 *   by colons. Each byte must be exactly 2 hexadecimal characters long and
 *   followed by a colon (except for the last byte). The minimum length of a
 *   hexstr is 2 bytes, as it must include at least a single colon.
 *   
 *   Examples: `de:ad:be:ef` `00:00`
 *
 * - **STRING**
 *
 *   A string literal starts and ends with a single quote. All characters within
 *   this literal are sent as-is, and no escaping is possible.
 *   
 *   Examples: <code>'Hello world!'</code> <code>'string'</code> <code>'  hello
 *   '</code>
 *   
 *   When double quotes are used instead of single quotes, the following escape
 *   sequences are recognised and replaced with special characters:
 *   |input|replacement|meaning|
 *   |-|-|-|
 *   |`\0`|0x00|null|
 *   |`\t`|0x09|tab|
 *   |`\n`|0x0a|newline|
 *   |`\r`|0x0d|carriage return|
 *   |`\\`|0x5c|backslash|
 *   |`\"`|0x22|double quote|
 *   |<code>\'</code>|0x27|single quote|
 *
 *   Examples: `"Hello world!\\0"` `"foo\\nbar"`
 * \endparblock
 */
cmd_handle_t cmd_send;
/**
 * \brief \c skip command
 *
 * ```
 * (pbc) skip <mod>
 * ```
 * 
 * Set a specific puzzle module's global state to *solved*. *mod* is the I2C
 * address of the puzzle module to skip. This parameter may be specified in
 * hexadecimal using a *0x* prefix or decimal.
 *
 */
cmd_handle_t cmd_skip;
/**
 * \brief \c dump command
 *
 * \note This command is only available in builds of \c pbc compiled with the
 * \c DEBUG flag
 *
 * ```
 * (pbc) dump <none|send|recv|both>
 * ```
 * 
 * Show raw I2C messages being sent to/from the main controller.
 */
cmd_handle_t cmd_dump;

/// \}

//! \c dump completion function
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
		.info = "set a puzzle module's game state to 'idle'",
	},
	{
		.handle = cmd_skip,
		.name = "skip",
		.info = "set a puzzle module's game state to 'solved'",
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
