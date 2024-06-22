#pragma once

/**
 * \ingroup main_tasks
 * \{
 */

/**
 * \brief Listen for TCP socket messages
 *
 * This task starts a TCP server that listens for messages using \ref i2ctcp,
 * and sends any received I2C messages (also using \ref i2ctcp).
 */
void serve_task();

/// \}

