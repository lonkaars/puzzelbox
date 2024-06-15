#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//! Arduino init variant (called before user setup)
void initVariant(void);

/**
 * \brief Arduino loop function
 *
 * Loop won't run because everything is handled by the freertos scheduler. It
 * is defined in this driver to cause compiler warnings if the user has defined
 * the loop() function anyways.
 */
extern void loop(void);

#ifdef __cplusplus
}
#endif

