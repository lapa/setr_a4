/**
 * @file UART.h
 * @brief UART configuration and command processing
 *
 * This file contains the declarations and configurations for UART communication.
 * It includes functions for configurating/initializing the UART, sending commands received to
 * the FIFO and processing those commands.
 * 
 * @author Diogo Lapa 117296
 * @author Bruno Duarte 118326
 * @date 04-06-2024
 * @bug No bugs to report for now
 * 
 */

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

/**
 * @struct uart_data_item_t
 * 
 * @brief Structure representing a UART data item.
 *
 * This structure holds the data and metadata for a UART data item,
 * including a reserved space for FIFO management, a buffer for received
 * characters, and indices to track the start and end of the buffer.
 * 
 */
struct uart_data_item_t {
    void *fifo_reserved; /* 1st word reserved for use by FIFO */
    uint8_t rx_chars[RXBUF_SIZE]; 
    int rx_buf_start;
    int rx_buf_end;
};

/**
 * @brief UART callback function to handle events.
 *
 * This function is called when a UART event occurs. It processes the event
 * based on its type and performs the necessary actions such as handling
 * transmission and reception of data, managing the RX buffer, and maintaining
 * the FIFO for received data.
 *
 * @param dev Pointer to the UART device structure.
 * @param evt Pointer to the UART event structure containing event type and data.
 * @param user_data Pointer to user data, if any.
 *
 * @warning This function may call `k_malloc` and `k_fifo_put`, ensure the necessary resources are available.
 */
void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

/**
 * @brief Initialize the UART device.
 *
 * This function initializes the UART device by configuring it, setting up
 * the callback function for handling UART events, and enabling data reception.
 *
 * @return uint16_t 
 * - Returns 1 on successful initialization.
 * - Returns FATAL_ERR if an error occurs during initialization.
 *
 */
uint16_t uart_init();

/**
 * @brief Validate Command
 *
 * Validates whether the given command is valid or not.
 * This function checks if the command is recognized and conforms to the expected format.
 *
 * @param command String representing the command to be validated.
 *
 * @return Indicates the result of the command validation.
 *         REGEX_COMPILE_FAIL - fails to compile the regex pattern.
 *         VALID_COMMAND - the command is valid.
 *         INVALID_COMMAND - the command is invalid.
 *         REGEX_MATCH_FAIL - fails the matching process.
 *
 */
uint16_t validate_command(char *command);

/**
 * @brief Validate Checksum
 *
 * Validates the checksum of the given command to ensure data integrity.
 * This function computes the checksum of the command and compares it with the provided checksum value.
 *
 * @param command String representing the command with its checksum.
 * @param rx_occupied_bytes Lengh of the command.
 *
 * @return Indicates the result of the checksum validation.
 *         CHECKSUM_MATCH - the checksum provided matches.
 *         CHECKSUM_MISMATCH - the checksum provided does not match.  
 * 
 */
uint16_t validate_checksum(char *command, uint16_t rx_occupied_bytes);

/**
 * @brief Thread function to process UART data from the FIFO.
 *
 * This thread function continuously retrieves UART data items from a FIFO,
 * extracts commands from the received data, and processes the commands.
 *  
 * The supported commands are:
 *      - 'B': Read the status of a button.
 *      - 'L': Read or set the status of an LED.
 *      - 'A': Read ADC values (raw or processed).
 * 
 * @param argA Unused parameter.
 * @param argB Unused parameter.
 * @param argC Unused parameter.
 *
 * @warning This function uses dynamic memory allocation with `malloc` and `free`.
 * Ensure sufficient heap memory is available and consider potential fragmentation.
 */
void fifo_thread_code(void *argA , void *argB, void *argC);

#endif