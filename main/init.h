#pragma once

#include <FreeRTOS.h>
#include <event_groups.h>

/**
 * \brief init function complete event group handle
 *
 * This is required to make sure the main task waits until initialization is
 * complete. Due to the combination of FreeRTOS + lwIP, the initialization
 * should be done while the task scheduler is running. Specifically the
 * cyw43_arch_init functions make the pico hang indefinitely when used while
 * the task scheduler is not running.
 *
 * \note `init_complete` only utilizes LSB, so `uxBitsToWaitFor` should always
 * be set to *1*
 */
extern EventGroupHandle_t init_complete;

/**
 * \brief initialize all peripherals on the pico
 *
 * This function only synchronously initializes the standard input/output (used
 * for `printf` and `panic`), and queues all other types of initialization in
 * the `init` task using FreeRTOS.
 *
 * \note Tasks dependent on the wifi being initialized should use the
 * `init_complete` event group to wait for initialization to complete!
 */
void init();

/**
 * \brief block task until all initialization is complete
 *
 * utility function, see above comments
 */
void await_init();

