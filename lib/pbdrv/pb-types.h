#pragma once

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
	PB_CMD_REQ_READ, //!< request a puzzle module property
	PB_CMD_RES_READ, //!< respond to a puzzle module property request
	PB_CMD_REQ_WRITE, //!< request to write a puzzle module property
	PB_CMD_REQ_STATE, //!< request global state
	PB_CMD_RES_STATE, //!< respond to a global state request
	PB_CMD_REQ_SET_STATE, //!< request to overwrite module global state
	PB_CMD_REQ_MAGIC, //!< magic request
	PB_CMD_RES_MAGIC, //!< magic response
};
typedef enum pb_cmd_id pb_cmd_id_t;

//! magic sent from main controller to puzzle module
static const char pb_cmd_magic_req[] = { 0x70, 0x75, 0x7a, 0x62, 0x75, 0x73 };
//! magic reply from puzzle module back to main controller
static const char pb_cmd_magic_res[] = { 0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67 };

//! puzzle bus global states
enum pb_global_state {
	PB_GS_NOINIT, //!< uninitialized (only used by puzzle modules)
	PB_GS_IDLE, //!< puzzle not started yet
	PB_GS_PLAYING, //!< puzzle actively being solved
	PB_GS_SOLVED, //!< puzzle completed
};
typedef enum pb_global_state pb_global_state_t;

//! puzzle bus message header (shared by all commands)
typedef struct {
	pb_cmd_id_t type; //!< command type
	i2c_addr_t sender; //!< i2c address of sender
	void * msg; //!< remaining message (type dependant)
} pb_msg_t;

//! PB_CMD_REQ_READ data
typedef struct {
	uint8_t propid; //!< state property id to return
} pb_cmd_req_read_t;

//! PB_CMD_RES_READ data
typedef struct {
	uint8_t propid; //!< id of returned state property
	uint8_t * value;
	size_t _value_size;
} pb_cmd_res_read_t;

//! PB_CMD_REQ_WRITE data
typedef struct {
	uint8_t propid; //!< state property id to write
	uint8_t * value; //!< new value of property
	size_t _value_size;
} pb_cmd_req_write_t;

//! PB_CMD_REQ_STATE data
typedef struct {
	pb_global_state_t state; //!< global state of sender
} pb_cmd_req_state_t;

//! PB_CMD_RES_STATE data
typedef struct {
	pb_global_state_t state; //!< global state of sender
} pb_cmd_res_state_t;

//! PB_CMD_REQ_SET_STATE data
typedef struct {
	pb_global_state_t state; //!< new global state
} pb_cmd_req_set_state_t;

//! PB_CMD_REQ_MAGIC data
typedef struct {
	char * magic; //!< magic value
	size_t _magic_size;
} pb_cmd_req_magic_t;

//! PB_CMD_RES_MAGIC data
typedef struct {
	char * magic; //!< magic value
	size_t _magic_size;
} pb_cmd_res_magic_t;

#ifdef __cplusplus
}
#endif

