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
	PB_CMD_READ, //!< read a puzzle module property
	PB_CMD_WRITE, //!< write to a puzzle module property
	// PB_CMD_UPDATE, //!< request an update
	PB_CMD_MAGIC = 0x69, //!< magic message
};
typedef enum pb_cmd pb_cmd_t;

static const char pb_magic_msg[] = { 0x70, 0x75, 0x7a, 0x62, 0x75, 0x73 };
static const char pb_magic_res[] = { 0x67, 0x61, 0x6d, 0x69, 0x6e, 0x67 };

/** \brief Puzzle bus global states */
enum __packed pb_state {
	PB_GS_NOINIT,  //!< uninitialized (only used by puzzle modules)
	PB_GS_IDLE,    //!< puzzle not started yet
	PB_GS_PLAYING, //!< puzzle actively being solved
	PB_GS_SOLVED,  //!< puzzle completed
};
typedef enum pb_state pb_state_t;

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

