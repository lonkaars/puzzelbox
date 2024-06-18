#include <FreeRTOS.h>

#include "../../pb-mem.h"
#include "../../pb-types.h"

__weak inline void * pb_malloc(size_t sz) {
	return pvPortMalloc(sz);
}

__weak inline void pb_free(void * ptr) {
	vPortFree(ptr);
}

__weak inline void * pb_realloc(void * ptr, size_t sz) {
	return NULL; // shit out of luck (don't use mpack_writer_init_growable)
}

__weak void * pb_memcpy(void * dest, const void * src, size_t sz) {
	for (size_t offset = 0; offset < sz; offset++)
		*((char*) dest + offset) = *((char*) src + offset);
	return dest;
}

int pb_memcmp(const void * a, const void * b, size_t sz) {
	for (size_t offset = 0; offset < sz; offset++) {
		int diff = *((char*) a + offset) - *((char*) b + offset);
		if (diff != 0) return diff;
	}
	return 0;
}

