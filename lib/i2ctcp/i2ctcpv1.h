#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup i2ctcp i2ctcp
 * \brief I2C over TCP
 *
 * This library includes protocol (de)serialization functions for sending and
 * receiving I2C messages over TCP. These functions are used by the \ref main
 * "main controller" and the \ref pbc "puzzle box client (pbc)". This library
 * does not include any puzzle bus specific code.
 *
 * mpack is used for the actual (de)serialization, and the functions in this
 * library act as helpers for parsing from chunked data streams.
 *
 * To use these functions, include the following statement in your
 * CMakeLists.txt:
 *
 * ```cmake
 * # include pbdrv
 * add_subdirectory(lib/i2ctcp)
 * 
 * # link with executable
 * target_link_libraries(main i2ctcp)
 * ```
 * 
 * \{
 */

//! I2C over TCP message (v1)
typedef struct {
	uint16_t addr; //!< I2C address
	char * data; //!< message content
	size_t length; //!< message size
	size_t
		_rdata; //!< \private remaining bytes to read until message is complete
} i2ctcp_msg_t;

/**
 * \brief Read chunk of input stream, and store resulting message in \p target
 *
 * This function is called for each chunk of data from an input stream, and
 * will parse the next puzzle bus message into \p target. The input stream is
 * assumed to only contain messages encoded by \c i2ctcp_write()
 *
 * \param target  Pointer to struct that will contain the finished message data
 * \param buf     Pointer to input stream data chunk
 * \param buf_sz  Size of \p buf
 *
 * \returns Integer representing amount of bytes required to finish message, or
 * -1 if the message header could not be read. If this function returns 0, the
 *  message in \p target is complete.
 *
 * \note \p target->data will automatically be allocated by this function, even
 * if the message is not fully parsed. This variable must be free'd by the
 * caller after each complete message to prevent memory leaks.
 */
int i2ctcp_read(i2ctcp_msg_t * target, const char * buf, size_t buf_sz);

/**
 * \brief Reset the remaining message data counter
 *
 * Calling this function has the effect of forcing \c i2ctcp_read() to parse
 * the next buffer chunk as the start of a new message. This function may be
 * called before reading a TCP frame's data to mitigate any synchronization
 * issues arising from earlier corrupt or otherwise malformed messages.
 */
void i2ctcp_read_reset(i2ctcp_msg_t * target);

/**
 * \brief Allocate and write a msgpack-formatted message to \p buf
 *
 * This function allocates a buffer large enough to fit the message specified
 * in \p target, and encodes the data in \p target in a format that can be
 * decoded later using \c i2ctcp_read().
 *
 * \param target  Pointer to struct that contains the message data
 * \param buf     Pointer to \c char* that will contain the formatted message
 * \param buf_sz  Pointer to \c size_t that will represent the final size of \p buf
 *
 * \returns Boolean \c true if a the message could be encoded successfully, \c
 * false if there was some kind of error
 *
 * \note The pointer stored in \p buf must be free'd by the caller afterwards
 */
bool i2ctcp_write(const i2ctcp_msg_t * target, char ** buf, size_t * buf_sz);

/// \}

#ifdef __cplusplus
}
#endif
