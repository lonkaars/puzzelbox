#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb_buf Buffer
 * \brief Binary data buffer type used in \ref pbdrv
 * \{
 */

//! binary buffer struct
typedef struct {
	char * data; //! pointer to data
	size_t size; //! size of data
} pb_buf_t;

/**
 * \brief free a \c pb_buf_t
 *
 * This function calls \c pb_free() on the \c data member of a \c pb_buf_t
 * struct if it is not equal to \c NULL.
 */
void pb_buf_free(pb_buf_t * buf);

/// \}

#ifdef __cplusplus
}
#endif

