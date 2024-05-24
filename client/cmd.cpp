#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "cmd.h"
#include "sock.h"

void cmd_exit(char*) {
	exit(EXIT_SUCCESS);
}

void cmd_test(char*) {
	const char* data = "Hello world!";
	i2c_send(0x39, (char*) data, strlen(data));
}

void cmd_help(char*) {
  printf("List of available commands:\n");
  for (size_t i = 0; i < cmds_length; i++) {
    struct cmd cmd = cmds[i];
    printf("  %-*s", 10, cmd.name);
    if (cmd.info != NULL)
      printf("  %s", cmd.info);
    printf("\n");
  }

  printf(
    "\n"
    "You can also use the TAB key to autocomplete commands\n"
  );
}

