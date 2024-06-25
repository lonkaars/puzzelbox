#pragma once

#include "pb-mem.h"

// use pb_* functions
#define MPACK_STDLIB 0

// pb_* memory management functions
#define MPACK_FREE pb_free
#define MPACK_MALLOC pb_malloc
#define MPACK_REALLOC pb_realloc
#define MPACK_MEMCPY pb_memcpy
#define MPACK_MEMCMP pb_memcmp

// more reasonable buffer size (all messages are small)
#define MPACK_BUFFER_SIZE 64
#define MPACK_STACK_SIZE MPACK_BUFFER_SIZE
#define MPACK_PAGE_SIZE MPACK_BUFFER_SIZE

// disable unused features (causes errors?)
#define MPACK_NODE 0
#define MPACK_BUILDER 0

