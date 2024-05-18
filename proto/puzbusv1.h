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
void pb_free(struct pb_msg* msg);

#ifdef __cplusplus
}
#endif

