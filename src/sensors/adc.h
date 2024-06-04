/**
 * @file adc.h
 * @brief This file contains the functions that allow us to read the values from the adc sensor
 * @Date 28-05-2024
 *
 */
#ifndef __ADC_H__
#define __ADC_H__

#include "common.h"

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
const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);	

/* Other defines */
#define TIMER_INTERVAL_MSEC 1000 /* Interval between ADC samples */

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

/* Error codes */
#define ERR_OK 0        // All fine
#define ERR_CONFIG -1   // Fail at set-up

/* 
 * Thread defines
 *
 * */

#define STACK_SIZE 1024
#define thread_ADC_prio 1 /* Higher priority */     
#define thread_ADC_period 1000
//K_TIMER_DEFINE(thread_ADC_timer, NULL, NULL);

static int adc_sample(void);
int read_value_adc(void);
int configure_adc(void);
void thread_ADC_code(void *argA, void *argB, void *argC);

#endif 
