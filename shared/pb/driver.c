#include <memory.h>

#include "types.h"
#include "driver.h"

/** \brief [private] placeholder global state variable */
static pb_state_t _global_state = PB_GS_NOINIT;

/** \brief [private] main controller global state */
static pb_state_t _main_state = PB_GS_NOINIT;

__weak pb_state_t pbdrv_hook_mod_state_read() {
	return _global_state;
}

__weak void pbdrv_hook_mod_state_write(pb_state_t state) {
	_global_state = state;
}

__weak void pbdrv_hook_main_state_update(pb_state_t state) { }

__weak bool pbdrv_hook_cmd(uint16_t i2c_addr, pb_cmd_t cmd, const char * buf, size_t sz) {
	return false;
}

__weak void pbdrv_i2c_recv(uint16_t i2c_addr, const char * buf, size_t sz) {
	if (sz == 0) return;
	pb_cmd_t cmd = (enum pb_cmd) buf[0];

	// shift buffer pointer to only contain the puzzle bus message buf
	buf++;
	sz--;

	// allow user to implement custom commands
	if (pbdrv_hook_cmd(i2c_addr, cmd, buf, sz))
		return;

	switch (cmd) {
		case PB_CMD_READ: return pbdrv_handle_read(i2c_addr, buf, sz);
		case PB_CMD_WRITE: return pbdrv_handle_write(i2c_addr, buf, sz);
		case PB_CMD_MAGIC: return pbdrv_handle_magic(i2c_addr, buf, sz);
		case PB_CMD_SEX: return pbdrv_handle_sex(i2c_addr, buf, sz);
		default: return;
	}
}

__weak void pbdrv_handle_read(uint16_t i2c_addr, const char * buf, size_t sz) {
	if (sz == 0) return;
	pb_cmd_read_t * cmd = (pb_cmd_read_t *) buf;

	// allow user to addrimplement custom read handlers
	if (pbdrv_hook_read(i2c_addr, cmd->address))
		return;

	switch (cmd->address) {
		case PB_ADDR_GS: {
			char res[] = {
				PB_CMD_READ,
				PB_ADDR_GS,
				pbdrv_hook_mod_state_read(),
			};
			return pbdrv_i2c_send(i2c_addr, res, sizeof(res));
		}
		default: return;
	}
}

__weak void pbdrv_handle_write(uint16_t i2c_addr, const char * buf, size_t sz) {
	if (sz < 2) return; // must have address and at least 1 byte data
	pb_cmd_write_t * cmd = (pb_cmd_write_t *) buf;

	// allow user to implement custom read handlers
	if (pbdrv_hook_write(i2c_addr, cmd->address, (char *) cmd->data, sz - 1))
		return;

	switch (cmd->address) {
		case PB_ADDR_GS:
			pbdrv_hook_mod_state_write(cmd->data[0]);
			break;
		default: return;
	}
}

__weak void pbdrv_handle_magic(uint16_t i2c_addr, const char * buf, size_t sz) {
	if (sz != sizeof(pb_magic_msg)) return;
	if (memcmp(buf, pb_magic_msg, sizeof(pb_magic_msg)) != 0) return;

	size_t res_size = sizeof(pb_cmd_t) + sizeof(pb_magic_res);
	char res[res_size];
	res[0] = PB_CMD_MAGIC;
	memcpy(res, pb_magic_res, sizeof(pb_magic_res));

	pbdrv_i2c_send(i2c_addr, res, res_size);
}

__weak void pbdrv_handle_sex(uint16_t i2c_addr, const char * buf, size_t sz) {
	if (sz == 0) return;
	pb_cmd_sex_t * cmd = (pb_cmd_sex_t *) buf;

	// send own state
	char res[] = {
		PB_CMD_SEX,
		pbdrv_hook_mod_state_read(),
	};
	pbdrv_i2c_send(i2c_addr, res, sizeof(res));

	if (cmd->main_state == _main_state) return;
	// keep main controller state
	_main_state = cmd->main_state;
	// call update if main state changed
	pbdrv_hook_main_state_update(_main_state);
}

