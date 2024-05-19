#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct pb_msg {
	uint16_t addr;
	char* data;
	size_t length;
};

/**
 * \brief Read chunk of input stream, and store resulting message in \p target
 *
 * This function is called for each chunk of data from an input stream, and
 * will parse the next puzzle bus message into \p target. The input stream is
 * assumed to only contain messages encoded by \p pb_write()
 *
 * \param target  pointer to struct that will contain the finished message data
 * \param buf     pointer to input stream data chunk
 * \param buf_sz  size of \p buf
 *
 * \returns boolean true if a message was completely read, false if more data
 * is required
 *
 * \note target->data will automatically be allocated by this function, and
 * must be `free()`d by the caller when finished
 */
bool pb_read(struct pb_msg* target, char* buf, size_t buf_sz);
/**
 * \brief Allocate and write a msgpack-formatted message to \p buf
 *
 * This function allocates a buffer large enough to fit the message specified
 * in \p target, and encodes the data in \p target in a format that can be
 * decoded later using \p pb_read()
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
bool pb_write(struct pb_msg* target, char** buf, size_t* buf_sz);

#ifdef __cplusplus
}
#endif

