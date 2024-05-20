#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "rl.h"

void rl_printf(const char *fmt, ...) {
	// save line
	char* saved_line = rl_copy_text(0, rl_end);
	int saved_point = rl_point;
	int saved_end = rl_end;
	
	// clear line
	rl_save_prompt();
	rl_replace_line("", 0);
	rl_redisplay();

	// printf
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	// restore line
	rl_restore_prompt();
	rl_replace_line(saved_line, 0);
	rl_point = saved_point;
	rl_end = saved_end;
	rl_redisplay();

	free(saved_line);
}

int cli_main() {
	char* input = NULL;
	while (1) {
		if (input != NULL) free(input);
		input = readline(CLI_PROMPT);

		// exit on ^D or ^C (EOF)
		if (input == NULL) return EXIT_SUCCESS;

		// add non-empty line to history
		if (*input) add_history(input);

		if (strcmp(input, "exit") == 0) return EXIT_SUCCESS;
	}	

	return EXIT_SUCCESS;
}

