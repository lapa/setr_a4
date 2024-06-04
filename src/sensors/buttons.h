/**
 * @file buttons.h
 * @brief Functions for interacting with buttons.
 *
 * This header file contains functions and definitions for configuring and
 * reading button states. It provides an interface to interact with specific
 * buttons defined by aliases (BUT0, BUT1, BUT2, BUT3).
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 *
 */

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "commons.h"

#define BUT0_NODE DT_ALIAS(sw0)
#define BUT1_NODE DT_ALIAS(sw1)
#define BUT2_NODE DT_ALIAS(sw2)
#define BUT3_NODE DT_ALIAS(sw3)
#define ERR_RDY -1


/**
 * @brief Configures the button devices and starts the button reading thread.
 *
 * This function checks the readiness of each button device, configures them as
 * GPIO inputs, and creates a thread for reading button states periodically.
 *
 * @return int
 * - Returns 0 on successful configuration.
 * - Returns ERR_RDY (-1) if any button device is not ready.
 *
 * @note Ensure that button devices (`but_0`, `but_1`, `but_2`, `but_3`) are properly defined and initialized
 *       before calling this function.
 * @note The button reading thread (`thread_button_read_code`) is created with the specified priority and stack size.
 *
 */
int configure_buttons(void);

/**
 * @brief Thread function for reading button states.
 *
 * This thread function continuously reads the state of buttons connected to GPIO pins,
 * updates their state in the real-time database (RTDB), and measures the execution time
 * of the button reading process.
 *
 * @param argA Unused parameter.
 * @param argB Unused parameter.
 * @param argC Unused parameter.
 *
 * @note The thread periodicity (thread_button_period) should be properly configured to avoid overrun or underrun.
 *
 */
void thread_button_read_code(void *argA, void *argB, void *argC);

#endif

