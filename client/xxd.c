#include <ctype.h>
#include <stdio.h>

#include "xxd.h"

void xxd(const char * data, size_t size) {
	size_t fake_size = size + (16 - size % 16) % 16;

	for (size_t base = 0; base < fake_size; base += 16) {
		printf("%08lx:  ", base);

		// print bytes
		for (size_t offset = 0; offset < 16; offset++) {
			size_t i = base + offset;

			if (offset == 8) printf(" ");

			if (i >= size) {
				printf("   ");
				continue;
			}

			printf("%02x ", data[i] & 0xff);
		}

		// print ascii representation
		printf(" |");
		for (size_t offset = 0; offset < 16; offset++) {
			size_t i = base + offset;

			if (i >= size) {
				printf(" ");
				continue;
			}

			if (isprint(data[i])) printf("%c", data[i]);
			else printf(".");
		}
		printf("|\n");
	}
}
