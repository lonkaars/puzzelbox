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
	// PB_CMD_UPDATE, //!< request an update
};

/** \brief Puzzle bus global states */
enum pb_global_state {
	PB_GS_NOINIT,  //!< uninitialized (only used by puzzle modules)
	PB_GS_IDLE,    //!< puzzle not started yet
	PB_GS_PLAYING, //!< puzzle actively being solved
	PB_GS_SOLVED,  //!< puzzle completed
};

