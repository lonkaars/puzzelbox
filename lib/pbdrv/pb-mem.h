#pragma once

#ifdef PB_TARGET_FREERTOS
#include <FreeRTOS.h>
#define pb_free(ptr) vPortFree(ptr)
#define pb_malloc(sz) pvPortMalloc(sz)
#define pb_realloc(ptr, sz) pvPortRealloc(ptr, sz)
#endif

#ifdef PB_TARGET_STDLIB
#include <stdlib.h>
#define pb_free(ptr) free(ptr)
#define pb_malloc(sz) malloc(sz)
#define pb_realloc(ptr, sz) realloc(ptr, sz)
#endif

