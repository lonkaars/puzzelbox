#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pb_msg {
	uint16_t addr;
	char* data;
	size_t length;
};

int pb_read(struct pb_msg* target, char* buf, size_t buf_sz);
int pb_write(struct pb_msg* target, char** buf, size_t* buf_sz);

#ifdef __cplusplus
}
#endif

