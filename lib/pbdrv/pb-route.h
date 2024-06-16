#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void pb_route_msg(pb_msg_t * msg);

bool pb_hook_route_msg(pb_msg_t * msg);

void pb_route_cmd_prop(pb_msg_t * msg);
void pb_route_cmd_state(pb_msg_t * msg);
void pb_route_cmd_magic(pb_msg_t * msg);

bool pb_hook_route_cmd_prop(pb_msg_t * msg);
bool pb_hook_route_cmd_state(pb_msg_t * msg);
bool pb_hook_route_cmd_magic(pb_msg_t * msg);

void pb_route_cmd_prop_req(pb_msg_t * msg);
void pb_route_cmd_prop_res(pb_msg_t * msg);
void pb_route_cmd_prop_set(pb_msg_t * msg);

void pb_route_cmd_state_req(pb_msg_t * msg);
void pb_route_cmd_state_res(pb_msg_t * msg);
void pb_route_cmd_state_set(pb_msg_t * msg);

void pb_hook_main_state_update(pb_global_state_t state);

void pb_route_cmd_magic_req(pb_msg_t * msg);
void pb_route_cmd_magic_res(pb_msg_t * msg);

#ifdef __cplusplus
}
#endif

