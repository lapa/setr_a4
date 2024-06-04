
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

#include "commons.h"

#define BUT0_NODE DT_ALIAS(sw0)
#define BUT1_NODE DT_ALIAS(sw1)
#define BUT2_NODE DT_ALIAS(sw2)
#define BUT3_NODE DT_ALIAS(sw3)
#define ERR_RDY -1


/**
 * @brief Sets the value of a specific LED
 *
 */
int configure_buttons(void);

#endif

