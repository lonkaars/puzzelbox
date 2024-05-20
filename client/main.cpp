#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <exception>

#include "rl.h"
#include "sock.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("usage: %s addr [port]\n", argv[0]);
		return EXIT_FAILURE;
	}

	// parse arguments
	char* addr = argv[1];
	uint16_t port = 9191;
	if (argc >= 3) port = atoi(argv[2]);

	try {
		// connect to TCP socket (automatically spawns thread)
		PBSocket sock(addr, port);
	} catch (const std::exception& e) {
		printf("error: %s\n", e.what());
		return EXIT_FAILURE;
	}

	// enter main CLI (using GNU readline for comfyness)
	return cli_main();
}

