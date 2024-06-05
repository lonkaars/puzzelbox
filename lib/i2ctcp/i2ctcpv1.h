#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief I2C over TCP message (v1) */
struct i2ctcp_msg {
	uint16_t addr; //!< I^2^C address
	char * data;   //!< message content
	size_t length; //!< message size
	size_t _rdata; //!< \private remaining bytes to read until message is complete
};
typedef struct i2ctcp_msg i2ctcp_msg_t;

/**
 * \brief Read chunk of input stream, and store resulting message in \p target
 *
 * This function is called for each chunk of data from an input stream, and
 * will parse the next puzzle bus message into \p target. The input stream is
 * assumed to only contain messages encoded by \p i2ctcp_write()
 *
 * \param target  pointer to struct that will contain the finished message data
 * \param buf     pointer to input stream data chunk
 * \param buf_sz  size of \p buf
 *
 * \returns Integer representing amount of bytes required to finish message, or
 * -1 if the message header could not be read. If this function returns 0, the
 *  message in \p target is complete.
 *
 * \note target->data will automatically be allocated by this function, even if
 * the message is not fully parsed. This variable must be `free()`d by the
 * caller after each complete message to prevent memory leaks.
 */
int i2ctcp_read(i2ctcp_msg_t * target, const char * buf, size_t buf_sz);

/**
 * \brief reset the remaining message data counter
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
 * decoded later using \p i2ctcp_read()
 *
 * \param target  pointer to struct that contains the message data
 * \param buf     pointer to \c char* that will contain the formatted message
 * \param buf_sz  pointer to \c size_t that will represent the final size of \p buf
 *
 * \returns boolean true if a the message could be encoded successfully, false
 * if there was some kind of error
 *
 * \note the pointer stored in \p buf must be `free()`d by the caller afterwards
 */
bool i2ctcp_write(const i2ctcp_msg_t * target, char ** buf, size_t * buf_sz);

#ifdef __cplusplus
}
#endif

