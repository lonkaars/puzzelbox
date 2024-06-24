#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbc
 * \defgroup pbc_xxd xxd
 * \brief Utility hexdump
 * \{
 */

/**
 * \brief utility function that prints hexdump of data
 */
void xxd(const char * data, size_t size);

/// \}

#ifdef __cplusplus
}
#endif

