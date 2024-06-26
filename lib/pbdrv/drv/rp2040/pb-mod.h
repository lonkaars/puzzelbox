#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup pb_drv_rp2040
 * \brief Puzzle bus driver setup
 *
 * This function must be called from a setup/init function to initialize the
 * I2C peripherals used by this driver.
 */
void pb_setup();

#ifdef __cplusplus
}
#endif

#include "../../pb-mod.h"
