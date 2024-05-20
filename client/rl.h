#pragma once

#define COLOR_OFF "\x1b[0m"
#define COLOR_BOLD "\x1b[1m"

#define CLI_PROMPT "(" COLOR_BOLD "pbc" COLOR_OFF ") "

#ifdef __cplusplus
extern "C" {
#endif

int cli_main();
void rl_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

