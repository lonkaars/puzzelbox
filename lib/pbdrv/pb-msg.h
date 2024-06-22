#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb_msg Message
 * \brief Message (de)serialization
 * \{
 */

/**
 * \brief Serialize a message into a binary buffer
 *
 * \note This function allocates a \c pb_buf_t that should be free'd using \c
 * pb_buf_free()
 *
 * \param msg Message to serialize
 *
 * \warning The type of \c msg->cmd is inferred from \c msg->type. If the
 * message is not correctly formatted, this function may cause undefined
 * behavior. If possible, use functions from \ref pb_send instead.
 *
 * \return \c pb_buf_t containing the serialized message, or an empty struct if
 * serialization failed
 */
pb_buf_t pb_msg_write(const pb_msg_t * msg);
/**
 * \brief Read a binary buffer and attempt to deserialize it as a puzzle bus
 * message
 *
 * \note This function allocates a \c pb_msg_t pointer that should be free'd
 * using \c pb_msg_free()
 *
 * \param buf Binary data to interpret as puzzle bus message
 *
 * \return \c pb_msg_t pointer containing the deserialized message, or NULL if
 * serialization failed
 */
pb_msg_t * pb_msg_read(const pb_buf_t * buf);
/**
 * \brief Recursively free fields of a \c pb_msg_t
 *
 * \note The \p msg pointer itself is also free'd by this function. You should
 * set it to NULL afterwards to avoid confusion.
 */
void pb_msg_free(pb_msg_t * msg);

/// \}

#ifdef __cplusplus
}
#endif

