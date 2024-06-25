#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "rl.h"
#include "cmd.h"
#include "parse.h"

static char* saved_line;
static int saved_point, saved_end;

void _rl_printf_start() {
	// save line
	saved_line = rl_copy_text(0, rl_end);
	saved_point = rl_point;
	saved_end = rl_end;

	// clear line
	rl_save_prompt();
	rl_replace_line("", 0);
	rl_redisplay();
}

void _rl_printf_stop() {
	rl_restore_prompt();
	rl_replace_line(saved_line, 0);
	rl_point = saved_point;
	rl_end = saved_end;
	rl_redisplay();

	free(saved_line);
}

void rl_printf(const char *fmt, ...) {
	_rl_printf_start();

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	_rl_printf_stop();
}

static void cli_cmd(char* cmd) {
	cmd += strspn(cmd, IFS); // skip leading whitespace
	char* line = consume_token(cmd, IFS);

	for (size_t i = 0; i < cmds_length; i++) {
		if (strncmp(cmds[i].name, cmd, strlen(cmd)) != 0)
			continue;

		cmds[i].handle(line);
		return;
	}

	printf("unknown command!\n");
}

static char* rl_completion_entries(const char *text, int state) {
	static size_t i = 0;
	if (state == 0) i = 0;

	while (i < cmds_length) {
		cmd_t cmd = cmds[i];
		i++;
		if (strncmp(text, cmd.name, strlen(text)) == 0) {
			return strdup(cmd.name);
		}
	}

	return NULL;
}

static char** rl_attempted_completion(const char * text, int start, int end) {
	// do not suggest filenames
	rl_attempted_completion_over = 1;

	// if first word in line buffer -> complete commands from cmds[]
	size_t cmd_start = strspn(rl_line_buffer, IFS);
	if (start == cmd_start)
		return rl_completion_matches(text, rl_completion_entries);

	// else, check specialized completion functions
	size_t cmd_len = strcspn(rl_line_buffer + cmd_start, IFS);
	for (size_t i = 0; i < cmds_length; i++) {
		cmd_t cmd = cmds[i];
		if (cmd.complete == NULL) continue;
		if (strncmp(cmd.name, rl_line_buffer + cmd_start, cmd_len) != 0) continue;
		return cmd.complete(text, start, end);
	}

	// else, no completion available
	return NULL;
}

int cli_main() {
	char* input = NULL;
	rl_attempted_completion_function = rl_attempted_completion;

	while (1) {
		if (input != NULL) free(input);
		input = readline(CLI_PROMPT);

		if (input == NULL) return EXIT_SUCCESS; // exit on ^D (EOF)
		if (*input == '\0') continue; // ignore empty lines
		add_history(input);

		cli_cmd(input);
	}	

	return EXIT_SUCCESS;
}

int rl_word(const char * line, int cursor) {
	int word = -1;
	for (int i = 0; line[i] != '\0';) {
		i += strspn(line + i, IFS);
		int len = strcspn(line + i, IFS);
		word++;
		i += len;
		if (i > cursor) break;
	}
	return word;
}

/// \internal
typedef struct {
	const char * word;
	const char ** options;
} __rl_complete_list_data_t;
char** rl_complete_list(const char * word, const char ** options) {
	__rl_complete_list_data_t data = {
		.word = word,
		.options = options,
	};
	return rl_completion_matches((char *) &data, [](const char * text, int state) -> char * {
		__rl_complete_list_data_t data = *(__rl_complete_list_data_t *) text;
		static size_t i = 0;
		if (state == 0) i = 0;

		while (data.options[i] != NULL) {
			const char * option = data.options[i++];
			if (strncmp(data.word, option, strlen(data.word)) == 0)
				return strdup(option);
		}
		return NULL;
	});
}

