#pragma once

#include "src/src/mpack/mpack.h"

/**
 * \brief read remaining bytes in reader without opening a tag first
 *
 * \param reader  pointer to mpack reader object
 * \param p  pointer to write data to
 * \param count  maximum number of bytes to read
 *
 * This function reads *up to* the amount of bytes specified in \p count, or
 * less if there is less remaining data in the buffer. If \p count is equal to
 * 0, all remaining data in the buffer is read.
 *
 * \return amount of bytes read
 */
size_t mpack_read_remaining_bytes(mpack_reader_t * reader, char * p, size_t count);

