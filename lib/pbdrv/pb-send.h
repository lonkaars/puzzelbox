#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv-mod
 * \defgroup pb_send Send
 * \brief Functions for directly creating serialized message buffers
 * \{
 */

/**
 * \brief Utility function for replying to a message
 *
 * \param msg Message to reply to
 * \param reply Data to send as reply
 *
 * This function uses \c pb_i2c_send() to send \p reply to \p msg->sender.
 */
void pb_send_reply(const pb_msg_t * msg, const pb_buf_t * reply);

/**
 * \brief Create a serialized \ref PB_CMD_PROP "PROP" \ref PB_ACTION_REQ "REQ"
 * message
 *
 * \param propid Property ID to request
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_read_req(uint8_t propid);
/**
 * \brief Create a serialized \ref PB_CMD_PROP "PROP" \ref PB_ACTION_RES "RES"
 * message
 *
 * \param propid Requested property ID
 * \param value Pointer to structured data in property
 * \param size Size of \p value
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_read_res(uint8_t propid, const uint8_t * value, size_t size);
/**
 * \brief Create a serialized \ref PB_CMD_PROP "PROP" \ref PB_ACTION_SET "SET"
 * message
 *
 * \param propid Property ID to write
 * \param value Pointer to data to write to property
 * \param size Size of \p value
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_write_req(uint8_t propid, const uint8_t * value, size_t size);
/**
 * \brief Create a serialized \ref PB_CMD_STATE "STATE" \ref PB_ACTION_REQ
 * "REQ" message
 *
 * The current module's state is obtained using \c pb_hook_mod_state_read().
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_state_req();
/**
 * \brief Create a serialized \ref PB_CMD_STATE "STATE" \ref PB_ACTION_RES
 * "RES" message
 *
 * The current module's state is obtained using \c pb_hook_mod_state_read().
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_state_res();
/**
 * \brief Create a serialized \ref PB_CMD_STATE "STATE" \ref PB_ACTION_SET
 * "SET" message
 *
 * \param state Requested new state
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_state_set(pb_global_state_t state);
/**
 * \brief Create a serialized \ref PB_CMD_MAGIC "MAGIC" \ref PB_ACTION_REQ
 * "REQ" message
 *
 * The magic string is equal to \ref pb_cmd_magic_req.
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_magic_req();
/**
 * \brief Create a serialized \ref PB_CMD_MAGIC "MAGIC" \ref PB_ACTION_RES
 * "RES" message
 *
 * The magic string is equal to \ref pb_cmd_magic_res.
 *
 * \return Message buffer
 *
 * \note The buffer returned by this function must be free'd with \c
 * pb_buf_free().
 */
pb_buf_t pb_send_magic_res();

/// \}

#ifdef __cplusplus
}
#endif

