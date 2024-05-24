#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "rl.h"
#include "cmd.h"

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

static bool cli_cmd(char* line) {
  char* cmd = strtok(line, " \t\n");
	for (size_t i = 0; i < cmds_length; i++) {
		if (strncmp(cmds[i].name, cmd, strlen(cmd)) != 0) continue;
		cmds[i].handle(line);
		return true;
	}
	return false;
}

static char* rl_completion_entries(const char *text, int state) {
  static size_t i = 0;
  if (state == 0) i = 0;

  while (i < cmds_length) {
    struct cmd cmd = cmds[i];
    i++;
    if (strncmp(text, cmd.name, strlen(text)) == 0) {
      return strdup(cmd.name);
    }
  }

  return NULL;
}

int cli_main() {
	char* input = NULL;
  rl_completion_entry_function = rl_completion_entries;

	while (1) {
		if (input != NULL) free(input);
		input = readline(CLI_PROMPT);

		if (input == NULL) return EXIT_SUCCESS; // exit on ^D (EOF)
		if (*input == '\0') continue; // ignore empty lines
		add_history(input);

		if (cli_cmd(input)) continue;

		printf("unknown command!\n");
	}	

	return EXIT_SUCCESS;
}

