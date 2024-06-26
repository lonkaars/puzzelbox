#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb_mem Memory
 * \brief Platform-specific memory management functions
 *
 * \note This header only declares the memory management functions, and it is
 * up to \ref pb_ext "extensions" to implement the underlying memory management
 * functions.
 *
 * TODO: ref to extensions
 *
 * \{
 */

/**
 * \brief Allocate a contiguous chunk of memory
 * \param sz Requested size of memory area
 * \return Pointer to memory area, or \c NULL on failure
 * \note The allocated memory must be free'd again using \c pb_free()
 */
void * pb_malloc(size_t sz);
/**
 * \brief Free a chunk of memory previously allocated with \c pb_malloc()
 * \param ptr Pointer to memory area
 */
void pb_free(void * ptr);
/**
 * \brief Resize the memory area \c ptr to size \c sz
 * \param ptr Pointer to allocated memory area
 * \param sz Requested new size of memory area
 * \return Pointer to memory area, or \c NULL on failure
 * \warning This function is not available on FreeRTOS, and should be avoided
 * if possible. MPack's \c mpack_writer_init_growable() is known to use \c
 * realloc(), and should not be used for this reason.
 */
void * pb_realloc(void * ptr, size_t sz);

/**
 * \brief copy a memory region
 * \param dest Pointer to destination memory
 * \param src Pointer to source memory
 * \param sz Number of bytes to copy
 * \return Pointer to \c dest
 *
 * This function has a portable implementation, and is always available.
 */
void * pb_memcpy(void * dest, const void * src, size_t sz);
/**
 * \brief compare two memory regions
 * \param a Pointer to first memory region
 * \param b Pointer to second memory region
 * \param sz Number of bytes to compare
 * \return 0 if the memory regions are identical, or the difference between the
 * first non-matching byte
 *
 * This function has a portable implementation, and is always available.
 */
int pb_memcmp(const void * a, const void * b, size_t sz);

/// \}

#ifdef __cplusplus
}
#endif
