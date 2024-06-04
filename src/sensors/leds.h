/**
 * @file leds.h
 * @brief Functions for interacting with LEDs on the Nordic Board.
 *
 * This header file contains functions and definitions for configuring and
 * controlling LEDs on the Nordic Board. It provides an interface to interact
 * with specific LEDs defined by aliases (LED0, LED1, LED2, LED3).
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 *
 */

#ifndef __LEDS_H__
#define __LEDS_H__

#include "commons.h"

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define ERR_RDY -1

/**
 * @brief Configures the LEDs for use.
 *
 * This function initializes and configures the GPIO pins corresponding to the
 * LEDs on the Nordic Board. It checks the readiness of each LED device and
 * configures them as GPIO outputs with an initial inactive state. Additionally,
 * it creates a thread (`thread_led_set_code`) to control the state of LEDs
 * periodically.
 *
 * @return int
 * - Returns 0 on success.
 * - Returns ERR_RDY (-1) if any LED device is not ready.
 *
 * @note Ensure that LED devices (led_0, led_1, led_2, led_3) are properly defined
 *       and initialized before calling this function.
 * @note The created thread (`thread_led_set_code`) should manage LED states
 *       based on the configuration set by this function.
 */
int configure_leds(void);

/**
 * @brief Thread function for setting LED states periodically.
 *
 * This thread function continuously reads the state of LEDs from the real-time
 * database (RTDB) and sets the corresponding GPIO pins to control the LEDs.
 * It measures the execution time of the LED setting process and operates in a
 * periodic manner based on the specified thread_led_period.
 *
 * @param argA Unused parameter.
 * @param argB Unused parameter.
 * @param argC Unused parameter.
 *
 * @note Ensure that GPIO pins and RTDB are properly configured and initialized
 *       before starting this thread (`thread_led_set_code`).
 * @note The thread periodicity (thread_led_period) should be properly configured to
 *       avoid overrun or underrun.
 *
 */
void thread_led_set_code(void *argA, void *argB, void *argC);

#endif

