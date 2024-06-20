#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define __weak __attribute__((weak))
#endif
#ifndef __weak
#error Could not determine weak attribute for current compiler
#define __weak
#endif

//! I2C address (10 or 7 bit)
typedef uint16_t i2c_addr_t;

//! puzzle bus command types
enum pb_cmd_id {
	/** \brief puzzle module property (REQ, RES, SET) */
	PB_CMD_PROP,
	/** \brief puzzle module global state variable (REQ, RES, SET) */
	PB_CMD_STATE,
	/**
	 * \brief magic (handshake) (REQ, RES)
	 *
	 * This message is used to distinguish between puzzle modules and regular I2C
	 * slaves on the puzzle bus.
	 */
	PB_CMD_MAGIC,
};
typedef enum pb_cmd_id pb_cmd_id_t;

//! puzzle bus command action types
enum pb_action {
	PB_ACTION_REQ, //!< request
	PB_ACTION_RES, //!< response
	PB_ACTION_SET, //!< (over)write
};
typedef enum pb_action pb_action_t;

//! puzzle bus global states
enum pb_global_state {
	PB_GS_NOINIT, //!< uninitialized (only used by puzzle modules)
	PB_GS_IDLE, //!< puzzle not started yet
	PB_GS_PLAYING, //!< puzzle actively being solved
	PB_GS_SOLVED, //!< puzzle completed
};
typedef enum pb_global_state pb_global_state_t;

//! magic sent from main controller to puzzle module
static const char pb_cmd_magic_req[] = { 0x70, 0x75, 0x7a, 0x62, 0x75, 0x73 };
//! magic reply from puzzle module back to main controller
static const char pb_cmd_magic_res[] = { 0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67 };

//! puzzle bus message header (shared by all commands)
typedef struct {
	/**
	 * \brief command type
	 *
	 * This is used to identify what the message is about.
	 */
	pb_cmd_id_t type;
	/**
	 * \brief command action
	 *
	 * This is used to specify what should happen as a result of this message.
	 */
	pb_action_t action;
	/**
	 * \brief I2C address of sender
	 *
	 * This is used to facilitate the 'network' features, as the sender of an I2C
	 * write is unknown.
	 */
	i2c_addr_t sender;
	/**
	 * \brief command data (type dependent)
	 *
	 * Struct containing command type-specific data.
	 */
	void * cmd;
} pb_msg_t;

//! PB_CMD_PROP data
typedef struct {
	uint8_t propid; //!< id of state property
	uint8_t * value; //!< new or current value
	size_t _value_size; //!< [META] size of \p value
} pb_cmd_prop_t;

//! PB_CMD_STATE data
typedef struct {
	pb_global_state_t state; //!< global state
} pb_cmd_state_t;

//! PB_CMD_MAGIC data
typedef struct {
	char * magic; //!< magic value
	size_t _magic_size; //!< [META] size of \p magic
} pb_cmd_magic_t;

#ifdef __cplusplus
}
#endif

