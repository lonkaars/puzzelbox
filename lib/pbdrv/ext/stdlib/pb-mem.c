#include <stdlib.h>
#include <string.h>

#include "../../pb-mem.h"

inline void * pb_malloc(size_t sz) {
	return malloc(sz);
}

inline void pb_free(void * ptr) {
	free(ptr);
}

inline void * pb_realloc(void * ptr, size_t sz) {
	return realloc(ptr, sz);
}

void * pb_memcpy(void * dest, const void * src, size_t sz) {
	return memcpy(dest, src, sz);
}

int pb_memcmp(const void * a, const void * b, size_t sz) {
	return memcmp(a, b, sz);
}

