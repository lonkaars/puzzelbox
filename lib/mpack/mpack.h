#pragma once

#include "src/src/mpack/mpack.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup mpack mpack
 * \brief Mpack extensions specific to this project
 *
 * The mpack folder under `lib/` contains a CMakeLists.txt for integrating the
 * mapck library with CMake, and includes an extension in the form of an
 * additional utility function.
 *
 * \{
 */

/**
 * \brief Read remaining bytes in reader without opening a tag first
 *
 * \param reader  Pointer to mpack reader object
 * \param p  Pointer to write data to
 * \param count  Maximum number of bytes to read
 *
 * This function reads *up to* the amount of bytes specified in \p count, or
 * less if there is less remaining data in the buffer. If \p count is equal to
 * 0, all remaining data in the buffer is read.
 *
 * \return Amount of bytes read
 */
size_t mpack_read_remaining_bytes(mpack_reader_t * reader, char * p,
								  size_t count);

/// \}

#ifdef __cplusplus
}
#endif
