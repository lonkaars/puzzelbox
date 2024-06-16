#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

//! binary buffer struct
typedef struct {
	char * data; //! pointer to data
	size_t size; //! size of data
} pb_buf_t;

void pb_buf_free(pb_buf_t * buf);

#ifdef __cplusplus
}
#endif

