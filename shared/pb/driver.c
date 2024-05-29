#include "types.h"
#include "driver.h"

__weak bool pbdrv_hook_cmd() {
	return false;
}

__weak void pbdrv_i2c_recv(uint16_t addr, const char * buf, size_t sz) {
	if (sz == 0) return;
	pb_cmd_t cmd = (enum pb_cmd) buf[0];

	// shift buffer pointer to only contain the puzzle bus message buf
	buf++;
	sz--;

	// allow user to override command handler while still using this weak
	// function
	if (pbdrv_hook_cmd(cmd, buf, sz)) return;

	switch (cmd) {
		case PB_CMD_READ: return pbdrv_handle_read(buf, sz);
		// case PB_CMD_WRITE: return pbdrv_handle_write(buf, sz);
		// case PB_CMD_MAGIC: return pbdrv_handle_magic(buf, sz);
		default: return;
	}
}

__weak void pbdrv_i2c_send(uint16_t addr, const char * buf, size_t sz) {
	return;
}

