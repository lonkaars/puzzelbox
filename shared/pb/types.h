#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define __packed __attribute__((packed))
#define __weak __attribute__((weak))
#endif
#ifndef __packed
#error Could not determine packed attribute for current compiler
#define __packed
#endif
#ifndef __weak
#error Could not determine weak attribute for current compiler
#define __weak
#endif

/**
 * \brief puzzle bus command types
 *
 * The first byte of a puzzle bus message's data indicates the command type.
 */
enum __packed pb_cmd {
	PB_CMD_READ,   //!< read a puzzle module property
	PB_CMD_WRITE,  //!< write to a puzzle module property
	// PB_CMD_UPDATE, //!< request an update
};

/** \brief Puzzle bus global states */
enum __packed pb_global_state {
	PB_GS_NOINIT,  //!< uninitialized (only used by puzzle modules)
	PB_GS_IDLE,    //!< puzzle not started yet
	PB_GS_PLAYING, //!< puzzle actively being solved
	PB_GS_SOLVED,  //!< puzzle completed
};

typedef struct __packed {
	uint8_t address;
} pb_cmd_read_t;

typedef struct __packed {
	uint8_t address;
	uint8_t data[];
} pb_cmd_write_t;

#ifdef __cplusplus
}
#endif

