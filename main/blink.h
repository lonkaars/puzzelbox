#pragma once

/**
 * \ingroup main_tasks
 * \{
 */

/**
 * \brief Status LED blink task
 *
 * This task is started after initialization is complete, and blinks the
 * on-board LED to indicate the Pico has completed initialization successfully.
 */
void blink_task();

/// \}
