#pragma once

/** \file bus address reference */

// Adafruit NeoTrellis modules
#define BUSADDR_ADA_NEO_1 0x2E
#define BUSADDR_ADA_NEO_2 0x2F
#define BUSADDR_ADA_NEO_3 0x30
#define BUSADDR_ADA_NEO_4 0x32

// TODO: ???
#define BUSADDR_MOD_NEOTRELLIS 0
#define BUSADDR_MOD_SOFTWARE 0
#define BUSADDR_MOD_HARDWARE 0
#define BUSADDR_MOD_VAULT 0
#define BUSADDR_MOD_AUTOMATION 0

// main controller
#define BUSADDR_MAIN 0x00

/**
 * \brief puzzle bus command types
 *
 * The first byte of a puzzle bus message's data indicates the command type.
 */
enum pb_cmd {
	PB_CMD_READ,   //!< read a puzzle module property
	PB_CMD_WRITE,  //!< write to a puzzle module property
	PB_CMD_EXCHANGE, //!< state exchange command
	// PB_CMD_UPDATE, //!< request an update
	PB_CMD_MAGIC = 0x69, //!< magic message
};
typedef enum pb_cmd pb_cmd_t;

static const char pb_magic_msg[] = { 0x70, 0x75, 0x7a, 0x62, 0x75, 0x73 };
static const char pb_magic_res[] = { 0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67 };

/** \brief Puzzle bus global states */
enum pb_state {
	PB_STATE_NOINIT,  //!< uninitialized (only used by puzzle modules)
	PB_STATE_IDLE,    //!< puzzle not started yet
	PB_STATE_PLAYING, //!< puzzle actively being solved
	PB_STATE_SOLVED,  //!< puzzle completed
};
typedef enum pb_state pb_state_t;

