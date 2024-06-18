#include "pb-mem.h"

#ifdef PB_TARGET_FREERTOS
#include <FreeRTOS.h>
#define _pb_free vPortFree
#define _pb_malloc pvPortMalloc
#endif

#ifdef PB_TARGET_STDLIB
#include <stdlib.h>
#define _pb_free free
#define _pb_malloc malloc
#define _pb_realloc realloc
#endif

inline void * pb_malloc(size_t sz) {
	return _pb_malloc(sz);
}

inline void pb_free(void * ptr) {
	_pb_free(ptr);
}

inline void * pb_realloc(void * ptr, size_t sz) {
#ifdef _pb_realloc
	return _pb_realloc(ptr, sz);
#else
	return NULL; // shit out of luck
#endif
}

