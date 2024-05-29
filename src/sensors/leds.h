/**
 * @file leds.h
 * @brief This file contains the functions that allow us to interact with the leds in the
 * Nordic Board
 *
 * @date 28-05-2024
 *
 */
#ifndef __LEDS_H__
#define __LEDS_H__

/**
 * @brief Reads the value of a specific LED and returns it
 */
int read_led(int id, int *res);


/**
 * @brief Sets the value of a specific LED
 *
 */
int set_led(int id);

#endif

