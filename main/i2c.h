#pragma once

/**
 * \ingroup main_tasks
 * \{
 */

/**
 * \brief I2C bus activity task 
 *
 * This function does an initial bus scan for puzzle modules, and then goes
 * into an infinite loop that periodically polls all puzzle modules for their
 * global state.
 */
void bus_task();

/// \}

