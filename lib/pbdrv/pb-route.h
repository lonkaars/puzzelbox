#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pbdrv-mod
 * \defgroup pb_route Routing
 * \internal
 * \brief Parsed message handler routing
 *
 * These functions form a tree-shaped call graph, and are used to handle
 * specific commands received from \c pb_i2c_recv().
 *
 * \{
 */

/**
 * \brief Handle a message with type {\ref PB_CMD_PROP "PROP", \ref
 * PB_CMD_STATE "STATE", \ref PB_CMD_MAGIC "MAGIC"}
 *
 * Calls the next handler depending on \c msg->type.
 */
void pb_route_msg(pb_msg_t * msg);

/**
 * \brief Handle a \ref PB_CMD_PROP "PROP" message with action {\ref
 * pb_route_cmd_prop_req "REQ", \ref pb_route_cmd_prop_res "RES", \ref
 * pb_route_cmd_prop_set "SET"}
 *
 * Calls the next handler depending on \c msg->action.
 */
void pb_route_cmd_prop(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_STATE "STATE" message with action {\ref
 * pb_route_cmd_state_req "REQ", \ref pb_route_cmd_state_res "RES", \ref
 * pb_route_cmd_state_set "SET"}
 *
 * Calls the next handler depending on \c msg->action.
 */
void pb_route_cmd_state(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_MAGIC "MAGIC" message with action {\ref
 * pb_route_cmd_magic_req "REQ", \ref pb_route_cmd_magic_res "RES"}
 *
 * Calls the next handler depending on \c msg->action.
 *
 * \note Messages with type \c MAGIC and action \c SET will be silently
 * ignored, as there is no such command.
 */
void pb_route_cmd_magic(pb_msg_t * msg);

/**
 * \brief Handle a \ref PB_CMD_PROP "PROP" message with action \ref
 * PB_ACTION_REQ "REQ"
 *
 * The default implementation of this function is empty, as puzzle module
 * properties are user-defined.
 */
void pb_route_cmd_prop_req(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_PROP "PROP" message with action \ref
 * PB_ACTION_RES "RES"
 *
 * The default implementation of this function is empty, as puzzle module
 * properties are user-defined.
 */
void pb_route_cmd_prop_res(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_PROP "PROP" message with action \ref
 * PB_ACTION_SET "SET"
 *
 * The default implementation of this function is empty, as puzzle module
 * properties are user-defined.
 */
void pb_route_cmd_prop_set(pb_msg_t * msg);

/**
 * \brief Handle a \ref PB_CMD_STATE "STATE" message with action \ref
 * PB_ACTION_REQ "REQ"
 *
 * The default implementation of this function does the following:
 * - Call \c pb_hook_ev_module_init() if this is the first received \c STATE \c
 *   REQ command.
 * - Call \c pb_hook_ev_main_state_update() if the main controller state has
 *   changed.
 * - Reply with a \c STATE \c RES message.
 */
void pb_route_cmd_state_req(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_STATE "STATE" message with action \ref
 * PB_ACTION_RES "RES"
 *
 * The default implementation of this function is empty, as only the main
 * controller handles this type of command.
 */
void pb_route_cmd_state_res(pb_msg_t * msg);
// TODO: add link to pb_route_cmd_state_res handler in main/i2c.c
/**
 * \brief Handle a \ref PB_CMD_STATE "STATE" message with action \ref
 * PB_ACTION_SET "SET"
 *
 * The default implementation of this function does the following:
 * - Write the global state variable using \c pb_hook_mod_state_write().
 */
void pb_route_cmd_state_set(pb_msg_t * msg);

/**
 * \brief Handle a \ref PB_CMD_MAGIC "MAGIC" message with action \ref
 * PB_ACTION_REQ "REQ"
 *
 * The default implementation of this function does the following:
 * - Verify the size of the magic string
 * - Verify the content of the magic string
 * - Reply with a \c MAGIC \c RES message.
 */
void pb_route_cmd_magic_req(pb_msg_t * msg);
/**
 * \brief Handle a \ref PB_CMD_MAGIC "MAGIC" message with action \ref
 * PB_ACTION_RES "RES"
 *
 * The default implementation of this function is empty, as only the main
 * controller handles this type of command. (\ref main_route_cmd_magic_res
 * "link")
 */
void pb_route_cmd_magic_res(pb_msg_t * msg);

/// \}

/**
 * \ingroup pb_hook
 * \defgroup pb_hook_route Routing
 * \brief Conditionally use substitute or extend the built-in message handlers
 * \{
 */

/**
 * \brief \c pb_route_msg() hook
 *
 * The default implementation of this function immediately returns \c false.
 *
 * \return \c false if execution should continue to the default handler, or \c
 * true if it should stop (i.e. the message was handled).
 */
bool pb_hook_route_msg(pb_msg_t * msg);

//! \c pb_route_cmd_prop() hook \copydetails pb_hook_route_msg
bool pb_hook_route_cmd_prop(pb_msg_t * msg);
//! \c pb_route_cmd_state() hook \copydetails pb_hook_route_msg
bool pb_hook_route_cmd_state(pb_msg_t * msg);
//! \c pb_route_cmd_magic() hook \copydetails pb_hook_route_msg
bool pb_hook_route_cmd_magic(pb_msg_t * msg);

/// \}

/**
 * \ingroup pb_hook
 * \defgroup pb_hook_ev Events
 * \brief Functions called on puzzle bus-related events
 * \{
 */

/**
 * \brief Main controller state update hook
 *
 * The default implementation of this function is empty.
 *
 * \param state New state of main controller
 *
 * \note This function is also called when the first \c STATE \c REQ command is
 * received, as the main controller state variable used to check if the state
 * actually changed is initialized to \ref PB_GS_NOINIT. In this case, this
 * function is called *after* \c pb_hook_ev_module_init().
 */
void pb_hook_ev_main_state_update(pb_global_state_t state);
/**
 * \brief Module initialized hook
 *
 * The default implementation of this function calls \c
 * pb_hook_mod_state_write() with \ref PB_GS_IDLE.
 *
 * This function is called when the first \c STATE \c REQ command is received,
 * indicating this puzzle module has been registered successfully by the main
 * controller, and is now part of an active play session.
 */
void pb_hook_ev_module_init();

/// \}

#ifdef __cplusplus
}
#endif

