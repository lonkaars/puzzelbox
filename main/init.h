#pragma once

/**
 * \brief initialize the main controller
 *
 * This function only synchronously initializes the standard input/output (used
 * for `printf` and `panic`), and queues all other types of initialization in
 * the `init` task using FreeRTOS.
 */
void init();

