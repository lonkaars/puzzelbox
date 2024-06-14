#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//! binary buffer struct
typedef struct {
	char * data; //! pointer to data
	size_t size; //! size of data
} pbdrv_buf_t;

void pbdrv_buf_free(pbdrv_buf_t * buf);

#ifdef __cplusplus
}
#endif

