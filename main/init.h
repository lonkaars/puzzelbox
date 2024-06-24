#pragma once

/**
 * \ingroup main
 * \{
 */

/**
 * \brief Initialize the main controller
 *
 * This function only synchronously initializes the standard input/output (used
 * for \c printf() and \c panic()), and queues all other types of
 * initialization in the \c async_init() task using FreeRTOS.
 */
void init();

/// \}
