/**
 * @file leds.h
 * @brief Functions for interacting with LEDs.
 *
 * This header file contains functions and definitions for configuring and
 * controlling LEDs. It provides an interface to interact
 * with specific LEDs defined by aliases (LED0, LED1, LED2, LED3).
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 *
 */

#include "leds.h"

const struct gpio_dt_spec led_0 = GPIO_DT_SPEC_GET(LED0_NODE,gpios);
const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1_NODE,gpios);
const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED2_NODE,gpios);
const struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET(LED3_NODE,gpios);

#define STACK_SIZE 1024
#define thread_led_prio 1 /* Higher priority */     
#define thread_led_period 1000
K_THREAD_STACK_DEFINE(thread_led_stack, STACK_SIZE);
struct k_thread thread_led_data;
k_tid_t thread_led_tid;

void thread_led_set_code(void *argA, void *argB, void *argC) {

    timing_t fin_time=0, release_time=0;
    
    /* Variables to time execution */
    timing_t start_time, end_time;
    uint64_t total_cycles=0;
    uint64_t total_ns=0;

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_led_period;

    while(true){
        /* Get one sample, checks for errors and prints the values */
	start_time = timing_counter_get(); 
	
	int res = 0;
	int ret = 0;
	rtdb_read_led(0, &res);
    	ret = gpio_pin_set_dt(&led_0, res);    
	if (ret < 0) {
	    return;
    	}
	rtdb_read_led(1, &res);
    	ret = gpio_pin_set_dt(&led_1, res);    
	if (ret < 0) {
	    return;
    	}
	rtdb_read_led(2, &res);
    	ret = gpio_pin_set_dt(&led_2, res);    
	if (ret < 0) {
	    return;
    	}
	rtdb_read_led(3, &res);
    	ret = gpio_pin_set_dt(&led_3, res);    
	if (ret < 0) {
	    return;
    	}

        end_time = timing_counter_get();

        total_cycles = timing_cycles_get(&start_time, &end_time);
        total_ns = timing_cycles_to_ns(total_cycles);
       
        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time); /* There are other variants, k_sleep(), k_usleep(), ... */       
        }
        /* Update next release time instant*/
        release_time += thread_led_period;
        //k_msleep(TIMER_INTERVAL_MSEC);
    }
    timing_stop();
}

int configure_leds(void) {
    int ret = 0;
    if (!device_is_ready(led_0.port))  
    {
    printk("Fatal error: led0 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_1.port))  
    {
    printk("Fatal error: led1 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_2.port))  
    {
    printk("Fatal error: led2 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_3.port))  
    {
    printk("Fatal error: led3 device not ready!");
    	return ERR_RDY;
    }

    ret = gpio_pin_configure_dt(&led_0, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_1, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_2, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_3, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    thread_led_tid = k_thread_create(&thread_led_data, thread_led_stack,
        K_THREAD_STACK_SIZEOF(thread_led_stack), thread_led_set_code,
        NULL, NULL, NULL, thread_led_prio, 0, K_NO_WAIT);
    return 0;

}
