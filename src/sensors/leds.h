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

#include "commons.h"

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define ERR_RDY -1
/**
 * @brief Sets the value of a specific LED
 *
 */
int set_led(int id, int value);


/**
 * @brief Sets the value of a specific LED
 *
 */
int toggle_led(int id);

/**
 * @brief Reads the value of a specific LED and returns it
 */
void read_leds(int id, int *res);

/**
 * @brief Reads the value of a specific LED and returns it
 */
int configure_leds(void);
#endif

