#pragma once

#define COLOR_OFF "\x1B[0m"
#define COLOR_BLUE "\x1B[0;94m"

#define CLI_PROMPT COLOR_BLUE "pbc" COLOR_OFF "% "

#ifdef __cplusplus
extern "C" {
#endif

int cli_main();
void rl_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

