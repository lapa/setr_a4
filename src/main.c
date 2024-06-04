/*
 * Paulo Pedreiras, 2024/02
 * 
 * Demo of the use of the UART driver.  
 *      There are several APIs. The demo is based on the asynchronous API, which is very flexible and covers most of the use cases.
 *      The demo configures the UART and echoes the user input. 
 *      Received characters are accumulated during a period set by MAIN_SLEEP_TIME_MS
 *      All UART events are echoed 
 * 
 * Info source:
 *      https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/peripherals/uart.html?highlight=uart_configure#overview 
 *
 * prj.conf should be set as follows:
 *
 *      To enable UART
 *          CONFIG_SERIAL=y
 *          CONFIG_UART_ASYNC_API=y
 * 
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

    fifo_thread_tid = k_thread_create(&fifo_thread_data, fifo_thread_stack,
        K_THREAD_STACK_SIZEOF(fifo_thread_stack), fifo_thread_code,
        NULL, NULL, NULL, fifo_thread_priority, 0, K_NO_WAIT);

    //configure_adc();
    configure_leds();
    configure_buttons();

    /* Main loop */
    while(1) {
        k_msleep(MAIN_SLEEP_TIME_MS);
        
        /* Print string received so far. */
        /* Very basic implementation, just for showing the use of the API */
        /* E.g. it does not prevent race conditions with the callback!!!!*/
        //if(uart_rxbuf_nchar > 0) {
        //    rx_chars[uart_rxbuf_nchar] = 0; /* Terminate the string */
        //    uart_rxbuf_nchar = 0;           /* Reset counter */
        //
        //    sprintf(rep_mesg,"You typed [%s]\n\r",rx_chars);            
        //    
        //    err = uart_tx(uart_dev, rep_mesg, strlen(rep_mesg), SYS_FOREVER_MS);
        //    if (err) {
        //        printk("uart_tx() error. Error code:%d\n\r",err);
        //        return FATAL_ERR;
        //    }
        //}
        //printk(".\n");
    }
 
}
