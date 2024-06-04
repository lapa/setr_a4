
/**
 * @file rtdb.h
 * @brief Real-Time Database (RTDB) API header file.
 *
 * This header file declares functions for interacting with the RTDB, which is
 * responsible for managing and accessing real-time data such as ADC values, LED
 * statuses and button states.
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 * 
 */
#ifndef __RTDB_H__
#define __RTDB_H__

#include <zephyr/kernel.h>

/**
 * @brief Reads the raw ADC value from the RTDB.
 *
 * @param res Pointer to store the read ADC raw value.
 */
void rtdb_read_adc_raw(int *res);

/**
 * @brief Reads the analog value (in millivolts) from the RTDB.
 *
 * @param res Pointer to store the read ADC analog value.
 */
void rtdb_read_adc_an(int *res);

/**
 * @brief Reads the status of a specific LED from the RTDB.
 *
 * @param id ID of the LED to read (0-3).
 * @param res Pointer to store the read LED status.
 */
void rtdb_read_led(int id, int *res);

/**
 * @brief Reads the status of a specific button from the RTDB.
 *
 * @param id ID of the button to read (0-3).
 * @param res Pointer to store the read button status.
 */
void rtdb_read_button(int id, int *res);

/**
 * @brief Sets the raw ADC value in the RTDB.
 *
 * @param value Raw ADC value to set.
 */
void rtdb_set_adc_raw(int value);

/**
 * @brief Sets the analog ADC value (in millivolts) in the RTDB.
 *
 * @param value Analog ADC value to set.
 */
void rtdb_set_adc_an(int value);

/**
 * @brief Sets the status of a specific LED in the RTDB.
 *
 * @param id ID of the LED to set (0-3).
 * @param value Status value to set for the LED.
 */
void rtdb_set_led(int id, int value);

/**
 * @brief Sets the status of a specific button in the RTDB.
 *
 * @param id ID of the button to set (0-3).
 * @param value Status value to set for the button.
 */
void rtdb_set_button(int id, int value);

#endif
