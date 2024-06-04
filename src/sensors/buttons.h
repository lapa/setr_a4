
/**
 * @file buttons.h
 * @brief This file contains the functions that allow us to interact with the buttons in the
 * Nordic Board
 *
 * @date 28-05-2024
 *
 */
#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>		/* for DT_NODELABEL() */
#include <zephyr/drivers/gpio.h>    /* for GPIO api*/


#define BUT0_NODE DT_ALIAS(sw0)
#define BUT1_NODE DT_ALIAS(sw1)
#define BUT2_NODE DT_ALIAS(sw2)
#define BUT3_NODE DT_ALIAS(sw3)
#define ERR_RDY -1
/**
 * @brief Reads the value of a specific LED and returns it
 */
int read_button(int id, int *res);


/**
 * @brief Sets the value of a specific LED
 *
 */
int configure_buttons(void);

#endif

