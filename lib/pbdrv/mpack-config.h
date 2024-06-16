#pragma once

#include "pb-mem.h"

#define MPACK_FREE(ptr) pb_free(ptr)
#define MPACK_MALLOC(sz) pb_malloc(sz)
#define MPACK_REALLOC(ptr, sz) pb_realloc(ptr, sz)

