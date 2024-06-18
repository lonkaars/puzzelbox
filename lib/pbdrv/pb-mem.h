#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void * pb_malloc(size_t sz);
void pb_free(void * ptr);
void * pb_realloc(void * ptr, size_t sz);

#ifdef __cplusplus
}
#endif

