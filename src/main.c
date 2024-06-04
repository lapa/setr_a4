/** @file  main.c
 * @brief Main file that initializes everything necessary
 *
 * Initializes the UART and the FIFO thread responsible for processing commands,
 * configures the ADC/LEDS/Buttons and their respective thread. 
 *
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 *
 */

/* Includes. Add according to the resources used  */
#include "UART/UART.h"
#include "sensors/adc.h"
#include "sensors/leds.h"
#include "sensors/buttons.h"

#include <zephyr/kernel.h>          /* for kernel functions*/
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/timing/timing.h>   /* for timing services */

/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024
#define fifo_thread_priority 1 

K_THREAD_STACK_DEFINE(fifo_thread_stack, STACK_SIZE);
struct k_thread fifo_thread_data;
k_tid_t fifo_thread_tid;

/* Main function */
int main(void)
{
    
    /* UART initialization */
    uart_init();

    /* Creating FIFO thread */
    fifo_thread_tid = k_thread_create(&fifo_thread_data, fifo_thread_stack,
        K_THREAD_STACK_SIZEOF(fifo_thread_stack), fifo_thread_code,
        NULL, NULL, NULL, fifo_thread_priority, 0, K_NO_WAIT);

    /* Configuring ADC/LEDS/Buttons and creating their threads */
    configure_adc();
    configure_leds();
    configure_buttons();
 
}
