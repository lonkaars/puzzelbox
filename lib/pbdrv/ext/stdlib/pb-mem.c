#include <stdlib.h>
#include <string.h>

#include "../../pb-mem.h"

/// \ingroup pb_ext_stdlib
inline void * pb_malloc(size_t sz) { return malloc(sz); }

/// \ingroup pb_ext_stdlib
inline void pb_free(void * ptr) { free(ptr); }

/// \ingroup pb_ext_stdlib
inline void * pb_realloc(void * ptr, size_t sz) { return realloc(ptr, sz); }

/// \ingroup pb_ext_stdlib
void * pb_memcpy(void * dest, const void * src, size_t sz) {
	return memcpy(dest, src, sz);
}

/// \ingroup pb_ext_stdlib
int pb_memcmp(const void * a, const void * b, size_t sz) {
	return memcmp(a, b, sz);
}
