/** 
 * @file co2_levels.h
 * @brief This file contains the functions relative to the CO2 sensor
 * All the responses will be in ASCII.
 *
 * @author bruno duarte
 *
 * @date 02-03-2024
 * @bug no bugs to report for now
 *
 */
#ifndef __CO2LEVELS_H__
#define __CO2LEVELS_H__

#include "common_funcs.h"

/**
 * @brief Generates a CO2 levels value
 *
 * This function generates the value to simluate the reading of the sensor.
 * 
 * @return Returns a uint16_t that is 0 in case of SUCCESS, although the function should never fail.
 *
 */
uint16_t generate_co2();

/**
 * @brief Simulates the reading of a CO2 levels value
 *
 * This function calls the function to generate the value and then adds it to the 
 * history of the last 20 readings.
 * 
 * @return Returns a uint16_t that is 0 in case of SUCCESS, although the function should never fail.
 *
 */
uint16_t read_sensor_co2();


/**
 * @brief Cheks if there are more than 20 entries in the CO2 levels sensor history
 *
 * This function sees if there are more than 20 entries in the history
 * 
 * @return Returns a uint16_t that is 1 if there are more than 20 entries and 0 otherwise
 *
 */
uint16_t check_co2levels();

/**
 * @brief Resets the saved CO2 levels in the history.
 *
 * This function resets the history of saved CO2 levels.
 * 
 * @return Returns a uint16_t that is SUCCESS(0) if nothing fails, nothing should fail.
 *
 */
uint16_t reset_co2levels();

/**
 * @brief Reads a value from a specific index in the history
 *
 * This functions reads a value from the CO2 sensors history.
 * 
 * @param[in] index Index of the wanted value.
 * @param[in] value Pointer to where the co2 history is being read to.
 * 
 * @return Returns a uint16_t that is SUCCESS(0) if nothing fails, and 
 *      VALUE_NOT_AVAIABLE(3) if the index doesn't exist.
 *
 */
uint16_t get_co2_history(uint16_t index, uint16_t *value);

#endif
