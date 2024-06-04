
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

/**
 * @brief Reads the value of a specific LED and returns it
 */
int read_button(int id, int *res);


/**
 * @brief Sets the value of a specific LED
 *
 */
int init_buttons(int id);

#endif

