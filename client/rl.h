#pragma once

/**
 * \ingroup pbc
 * \defgroup pbc_rl rl
 * \brief GNU Readline related functions
 * \{
 */

//! Reset color (ANSI sequence)
#define COLOR_OFF "\x1b[0m"
//! Set font to bold (ANSI sequence)
#define COLOR_BOLD "\x1b[1m"

//! Prompt text
#define CLI_PROMPT "(" COLOR_BOLD "pbc" COLOR_OFF ") "

//! CLI entrypoint
int cli_main();

/**
 * \brief Print format string to stdout without disturbing the readline prompt
 *
 * This function saves and restores the current readline prompt before/after
 * calling printf. This function is not required for commands that print output
 * synchronously, as the prompt is only shown after a command handler
 * completes.
 */
void rl_printf(const char * fmt, ...);
//! Store line for printing without disrupting readline prompt
void _rl_printf_start();
//! Restore line stored by \c _rl_printf_start()
void _rl_printf_stop();

/**
 * \brief Get the index of the word currently under the cursor
 *
 * \param line Command line contents
 * \param cursor Index of cursor position
 *
 * This function returns the index of the word from an array made by splitting
 * \p line on consecutive occurrences of \c IFS.
 *
 * \return Index of word
 */
int rl_word(const char * line, int cursor);
/**
 * \brief Create a completion suggestion string array for readline
 *
 * \param word Word to complete
 * \param options List of possible choices (NULL terminated array of strings)
 *
 * \return Suggestions matching \p word
 */
char ** rl_complete_list(const char * word, const char * options[]);

/// \}
