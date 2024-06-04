
#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>		/* for DT_NODELABEL() */
#include <zephyr/drivers/gpio.h>    /* for GPIO api*/
#include <zephyr/drivers/adc.h>     /* for ADC API*/
#include <zephyr/sys/printk.h>      /* for printk()*/
#include <string.h>
#include <zephyr/timing/timing.h>   /* for timing services */
/*ADC definitions and includes*/
#include <hal/nrf_saadc.h>
#include "rtdb.h"
#endif
