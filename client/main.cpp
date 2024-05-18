#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "puzbusv1.h"

int send_message() {
	const char* data = "Test message data!";
	struct pb_msg output = {
		.addr = 0x39,
		.data = (char*) data,
		.length = strlen(data),
	};

	char* packed;
	size_t size;
	if (!pb_write(&output, &packed, &size)) {
		printf("error writing!\n");
		return 1;
	}

	fwrite(packed, sizeof(packed[0]), size, stdout);
	fflush(stdout);

	return 0;
}

int read_message() {
	freopen(NULL, "rb", stdin); // allow binary on stdin
	struct pb_msg input;
	
	char buf[8]; // extremely small buffer to test chunked message parsing
	size_t bytes = 0;
	while ((bytes = fread(buf, sizeof(buf[0]), sizeof(buf), stdin)) > 0) {
		if (!pb_read(&input, buf, bytes)) continue;

		printf("address: 0x%02x\n", input.addr);
		printf("data:    \"%.*s\"\n", input.length, input.data);
		free(input.data);
		return 0;
	}

	return 1;
}

int main() {
	if (!isatty(fileno(stdout))) return send_message();
	if (!isatty(fileno(stdin))) return read_message();
	
	printf("please pipe some data in or out to use this program\n");
	return 0;
}

