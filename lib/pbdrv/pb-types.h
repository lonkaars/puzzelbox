#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb_types Types
 * \brief Datatypes used within \ref pbdrv
 *
 * \{
 */

#ifdef __GNUC__
//! Mark function as weak (allow user to override implementation)
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
	/**
	 * \brief puzzle module property (\ref pb_route_cmd_prop_req "REQ", \ref
	 * pb_route_cmd_prop_res "RES", \ref pb_route_cmd_prop_set "SET")
	 *
	 * The \c PROP command type is used for exchanging arbitrary data between
	 * puzzle modules and/or the puzzle box client (pbc) over the TCP bridge.
	 * These properties are not used by the puzzle framework.
	 */
	PB_CMD_PROP,
	/**
	 * \brief puzzle module global state variable (\ref pb_route_cmd_state_req
	 * "REQ", \ref pb_route_cmd_state_res "RES", \ref pb_route_cmd_state_set
	 * "SET")
	 *
	 * The \c STATE command is used by puzzle modules to inform the main
	 * controller about their global state. The main controller aggregates the
	 * states of all connected puzzle modules and exchanges this aggregated state
	 * with the puzzle modules to indicate when the entire puzzle box is solved.
	 */
	PB_CMD_STATE,
	/**
	 * \brief magic (handshake) (\ref pb_route_cmd_magic_req "REQ", \ref
	 * pb_route_cmd_magic_res "RES")
	 *
	 * The \c MAGIC command effectively serves as a 'secret handshake' (using a
	 * _magic_ value) which is used to distinguish between puzzle modules and
	 * unrelated I2C devices.
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

//! puzzle bus message header / container (shared by all commands)
typedef struct {
	/**
	 * \brief Command type (see \ref pb_cmd_id_t)
	 *
	 * This is used to identify what the message is about.
	 */
	pb_cmd_id_t type;
	/**
	 * \brief Command action (see \ref pb_action_t)
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
	 * \brief Command data (dependent on \p type)
	 *
	 * Struct containing command type-specific data.
	 */
	void * cmd;
} pb_msg_t;

//! \ref PB_CMD_PROP data
typedef struct {
	uint8_t propid; //!< id of state property
	uint8_t * value; //!< new or current value
	size_t _value_size; //!< size of \p value
} pb_cmd_prop_t;

//! \ref PB_CMD_STATE data
typedef struct {
	pb_global_state_t state; //!< global state
} pb_cmd_state_t;

//! \ref PB_CMD_MAGIC data
typedef struct {
	char * magic; //!< magic value
	size_t _magic_size; //!< size of \p magic
} pb_cmd_magic_t;

/// \}

#ifdef __cplusplus
}
#endif

