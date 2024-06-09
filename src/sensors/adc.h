/**
 * @file adc.h
 * @brief Functions for reading values from the ADC sensor.
 *
 * This header file contains the declarations and definitions required
 * for configuring and reading values from the ADC (Analog-to-Digital Converter)
 * sensor. It includes configuration settings, constants, and function 
 * prototypes for sampling and reading ADC values.
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 *
 * @note Ensure that the ADC configuration parameters match your hardware specifications.
 *
 */

#ifndef __ADC_H__
#define __ADC_H__

#include "commons.h"


/*
 * ADC defines
 * */

#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40)
#define ADC_CHANNEL_ID 1  

/* This is the actual nRF ANx input to use. Note that a channel can be assigned to any ANx. In fact a channel can */
/*    be assigned to two ANx, when differential reading is set (one ANx for the positive signal and the other one for the negative signal) */  
/* Note also that the configuration of different channels is completely independent (gain, resolution, ref voltage, ...) */
#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1 

#define BUFFER_SIZE 1

#define ADC_NODE DT_NODELABEL(adc)  

/* Other defines */
#define TIMER_INTERVAL_MSEC 1000 /* Interval between ADC samples */

/* Error codes */
#define ERR_OK 0        // All fine
#define ERR_CONFIG -1   // Fail at set-up

/* 
 * Thread defines
 *
 * */

#define STACK_SIZE 1024
#define thread_ADC_prio 3 
#define thread_ADC_period 1000
//K_TIMER_DEFINE(thread_ADC_timer, NULL, NULL);

/**
 * @brief Sample the ADC and store the result in the buffer.
 *
 * This function performs an ADC sampling operation using the configured ADC
 * device and stores the sampled data in a buffer. It sets up an ADC sequence
 * structure, checks if the ADC device is properly bound, and initiates the
 * ADC read operation.
 *
 * @return int
 * - Returns 0 on successful sampling.
 * - Returns -1 if the ADC device is not bound.
 * - Returns a negative error code if the ADC read operation fails.
 *
 * @warning This function prints error messages to the console in case of failure.
 */
int adc_sample(void);

/**
 * @brief Configure the ADC device and setup the ADC sampling thread.
 *
 * This function sets up the ADC channel configuration, performs a calibration
 * of the ADC offset, and creates a thread for periodic ADC sampling.
 *
 * @return int
 * - Returns ERR_OK (0) on successful configuration.
 * - Returns ERR_CONFIG (-1) if there is an error during ADC channel setup.
 *
 * @warning This function prints error messages to the console in case of failure.
 */
int configure_adc(void);

/**
 * @brief ADC sampling thread function.
 *
 * This thread function periodically samples the ADC, processes the sampled data,
 * and stores the results in the real-time database (RTDB). It also measures the
 * execution time of the ADC sampling process.
 *
 * @param argA Unused parameter.
 * @param argB Unused parameter.
 * @param argC Unused parameter.
 *
 * The ADC is configured to use a gain of 1/4 and a reference voltage of VDD/4,
 * resulting in an input range of 0 to 3V with 10-bit resolution.
 *
 * @warning This function prints error messages to the console in case of failure.
 * @warning Ensure the thread periodicity (thread_ADC_period) is properly configured to avoid overrun or underrun.
 */
void thread_ADC_code(void *argA, void *argB, void *argC);

#endif 
