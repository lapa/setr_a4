#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

/* Includes. Add according to the resources used  */
#include <zephyr/kernel.h>          /* for k_msleep() */
#include <zephyr/device.h>          /* for device_is_ready() and device structure */
#include <zephyr/devicetree.h>	    /* for DT_NODELABEL() */
#include <zephyr/drivers/uart.h>    /* for UART API*/
#include <zephyr/sys/printk.h>      /* for printk()*/
#include <zephyr/drivers/gpio.h>
#include <zephyr/timing/timing.h>   /* for timing services */
#include <stdint.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>

#define SOF_SYM '#'         /* Start of Frame Symbol */
#define EOF_SYM '!'         /* End of Frame Symbol */
#define VALID_COMMAND 0
#define CHECKSUM_MATCH 0
#define INVALID_COMMAND 1
#define REGEX_COMPILE_FAIL 2
#define REGEX_MATCH_FAIL 3
#define CHECKSUM_MISMATCH 4

#define UART_NODE DT_NODELABEL(uart0)   /* UART0 node ID*/
#define MAIN_SLEEP_TIME_MS 1000 /* Time between main() activations */ 

#define FATAL_ERR -1 /* Fatal error return code, app terminates */

#define RXBUF_SIZE 60                   /* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define MSG_BUF_SIZE 100                /* Buffer for messages sent vai UART */
#define RX_TIMEOUT 1000                 /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

struct uart_data_item_t {
    void *fifo_reserved; /* 1st word reserved for use by FIFO */
    uint8_t rx_chars[RXBUF_SIZE];
    int rx_buf_start;
    int rx_buf_end;
};

/* UART callback function prototype */
void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);
uint16_t uart_init();
uint16_t uart_hello_message();
uint16_t validate_command(char *command);
uint16_t validate_checksum(char *command, uint16_t rx_occupied_bytes);

#endif