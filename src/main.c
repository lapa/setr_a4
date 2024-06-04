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

/* Main function */
int main(void)
{
    
    /* UART initialization */
    //uart_init();
    //uart_hello_message();

    configure_adc();

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
