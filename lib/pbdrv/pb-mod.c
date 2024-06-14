#include "pb-buf.h"
#include "pb-msg.h"
#include "pb-types.h"
#include "pb-mod.h"
#include "pb-route.h"

//! fallback module name
__weak const char * PB_MOD_NAME = "???";

//! [private] placeholder global state variable
static pb_global_state_t _global_state = PB_GS_NOINIT;

__weak pb_global_state_t pb_hook_mod_state_read() {
	return _global_state;
}

__weak void pb_hook_mod_state_write(pb_global_state_t state) {
	_global_state = state;
}

__weak void pb_i2c_recv(const uint8_t * data, size_t sz) {
	pb_buf_t buf = {
		.data = (char *) data,
		.size = sz,
	};
	pb_msg_t * msg = pb_msg_read(&buf);
	if (msg == NULL) return; // invalid message
	if (msg->cmd == NULL) return; // invalid message
	
	pb_route_msg(msg);

	pb_msg_free(msg);
}

void pb_reply(pb_msg_t * msg, pb_buf_t * reply) {
	return pb_i2c_send(msg->sender, (uint8_t *) reply->data, reply->size);
}

