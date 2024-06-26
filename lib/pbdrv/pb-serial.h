#pragma once

#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb_ser Serial
 * \internal
 * \brief Internal (de)serialization functions using mpack
 *
 * \{
 */

/**
 * \brief Write (serialize) message fields using mpack
 *
 * \param writer Pointer to \c mpack_writer_t instance
 * \param msg Pointer to message struct to read from
 */
typedef void pb_ser_w_t(mpack_writer_t * writer, const pb_msg_t * msg);
/**
 * \brief Read (deserialize) message fields using mpack
 *
 * \param reader Pointer to \c mpack_reader_t instance
 * \param msg Pointer to message struct to write to
 */
typedef void pb_ser_r_t(mpack_reader_t * reader, pb_msg_t * msg);
/**
 * \brief Recursively free message struct fields
 *
 * \param msg Pointer to message struct to free
 */
typedef void pb_ser_free_t(pb_msg_t * msg);

/**
 * \brief Write the \ref pb_msg_t header fields and call another function for
 * \p msg->cmd.
 * \see pb_ser_w_t
 */
pb_ser_w_t pb_ser_w;
/**
 * \brief Read the \ref pb_msg_t header fields and call another function for \p
 * msg->cmd.
 * \see pb_ser_r_t
 */
pb_ser_r_t pb_ser_r;
/**
 * \brief Call another function for \p msg->cmd.
 * \see pb_ser_free_t
 */
pb_ser_free_t pb_ser_free;

//! Write the \ref pb_cmd_prop_t fields \see pb_ser_w_t
pb_ser_w_t pb_ser_w_cmd_prop;
//! Read the \ref pb_cmd_prop_t fields \see pb_ser_r_t
pb_ser_r_t pb_ser_r_cmd_prop;
//! Free the \ref pb_cmd_prop_t fields \see pb_ser_free_t
pb_ser_free_t pb_ser_free_cmd_prop;

//! Write the \ref pb_cmd_state_t fields \see pb_ser_w_t
pb_ser_w_t pb_ser_w_cmd_state;
//! Read the \ref pb_cmd_state_t fields \see pb_ser_r_t
pb_ser_r_t pb_ser_r_cmd_state;
//! Free the \ref pb_cmd_state_t fields \see pb_ser_free_t
pb_ser_free_t pb_ser_free_cmd_state;

//! Write the \ref pb_cmd_magic_t fields \see pb_ser_w_t
pb_ser_w_t pb_ser_w_cmd_magic;
//! Read the \ref pb_cmd_magic_t fields \see pb_ser_r_t
pb_ser_r_t pb_ser_r_cmd_magic;
//! Free the \ref pb_cmd_magic_t fields \see pb_ser_free_t
pb_ser_free_t pb_ser_free_cmd_magic;

/// \}

#ifdef __cplusplus
}
#endif
