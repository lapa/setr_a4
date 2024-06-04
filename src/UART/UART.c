#include "UART.h"
#include "../sensors/leds.h"

/* UART related variables */
const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);
static uint8_t rx_buf[RXBUF_SIZE];        /* RX buffer, to store received data */
//static uint8_t rx_chars[RXBUF_SIZE];    /* chars actually received  */
volatile int uart_rxbuf_nchar = 0;        /* Number of chars currrntly on the rx buffer */
volatile int uart_rxbuf_start = 0;

/* Struct for UART configuration. If using default values (check devicetree info) is not needed) */
/* Dynamic configuration option, available if CONFIG_UART_USE_RUNTIME_CONFIGURE is ser (it is by defualt)*/
/* For static UART configuration it is recommended to use the devicetree editor (Actions tab) */
const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* Command processing variables */
static regex_t regex;
static char* command_pattern = "^#(B[1-4]|L([1-4]|[1-4][0-1])|A(R|V))(2[5][0-5]|2[0-4][0-9]|[0-1][0-9]{2})!$";

K_FIFO_DEFINE(uart_fifo);

/* UART callback implementation */
/* Note that callback functions are executed in the scope of interrupt handlers. */
/* They run asynchronously after hardware/software interrupts and have a higher priority than tasks/threads */
/* Should be kept as short and simple as possible. Heavier processing should be deferred to a task with suitable priority*/
void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    int err;

    switch (evt->type) {
	
        case UART_TX_DONE:
		    printk("UART_TX_DONE event \n\r");
            break;

    	case UART_TX_ABORTED:
	    	printk("UART_TX_ABORTED event \n\r");
		    break;
    
	    case UART_RX_RDY:
		    printk("UART_RX_RDY event \n\r");

            if(rx_buf[evt->data.rx.offset] == SOF_SYM) {
                uart_rxbuf_start = evt->data.rx.offset; 
            }

            size_t FIFO_elem_size = sizeof(struct uart_data_item_t);
            struct uart_data_item_t *item_ptr;

            item_ptr = k_malloc(FIFO_elem_size);
            item_ptr->rx_buf_start = uart_rxbuf_start;
            item_ptr->rx_buf_end = evt->data.rx.offset;
            memcpy(item_ptr->rx_chars, rx_buf, RXBUF_SIZE);

            k_fifo_put(&uart_fifo, item_ptr);
            
            break;

	    case UART_RX_BUF_REQUEST:
		    printk("UART_RX_BUF_REQUEST event \n\r");
            /* Should be used to allow continuous reception */
            /* To this end, declare at least two buffers and switch among them here */
            /*      using function uart_rx_buf_rsp() */
		    break;

	    case UART_RX_BUF_RELEASED:
		    printk("UART_RX_BUF_RELEASED event \n\r");
		    break;
		
	    case UART_RX_DISABLED: 
            /* When the RX_BUFF becomes full RX is disabled automaticaly.  */
            /* It must be re-enabled manually for continuous reception */
            printk("UART_RX_DISABLED event \n\r");
		    err = uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
            if (err) {
                printk("uart_rx_enable() error. Error code:%d\n\r",err);
                exit(FATAL_ERR);                
            }
		    break;

	    case UART_RX_STOPPED:
		    printk("UART_RX_STOPPED event \n\r");
		    break;
		
	    default:
            printk("UART: unknown event \n\r");
		    break;
    }

}

uint16_t uart_init() {

    /* Local vars */    
    int err = 0; /* Generic error variable */

    /* Check if uart device is open */
    if (!device_is_ready(uart_dev)) {
        printk("device_is_ready(uart) returned error! Aborting! \n\r");
        return FATAL_ERR;
    }

    /* Configure UART */
    err = uart_configure(uart_dev, &uart_cfg);
    if (err == -ENOSYS) { /* If invalid configuration */
        printk("uart_configure() error. Invalid configuration\n\r");
        return FATAL_ERR; 
    }

    /* Register callback */
    err = uart_callback_set(uart_dev, uart_cb, NULL);
    if (err) {
        printk("uart_callback_set() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }

    /* Enable data reception */
    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
    if (err) {
        printk("uart_rx_enable() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }

    return 1;
}

uint16_t uart_hello_message() {
    int err = 0; /* Generic error variable */
    uint8_t welcome_mesg[] = "TYPE A COMMAND!!!\n\r"; 

     /* Send a welcome message */ 
    /* Last arg is timeout. Only relevant if flow controll is used */
    err = uart_tx(uart_dev, welcome_mesg, sizeof(welcome_mesg), SYS_FOREVER_MS);
    if (err) {
        printk("uart_tx() error. Error code:%d\n\r",err);
        return FATAL_ERR;
    }
    return 1;
}

uint16_t validate_command(char *command) {
    
    uint16_t ret = regcomp(&regex, command_pattern, REG_EXTENDED);
    if(ret != 0) {
        return REGEX_COMPILE_FAIL;
    }

    ret = regexec(&regex, command, 0, NULL, 0);
    if(ret == 0) {
        return VALID_COMMAND;
    } else if(ret == REG_NOMATCH) {
        return INVALID_COMMAND;
    } 
    return REGEX_MATCH_FAIL;

}

uint16_t validate_checksum(char *command, uint16_t rx_occupied_bytes) {

    uint16_t start_commandsum = 1;
    uint16_t end_commandsum = rx_occupied_bytes - 5;
    uint16_t start_checksum = rx_occupied_bytes - 4;
    uint16_t end_checksum = rx_occupied_bytes - 2;

    uint16_t checksum = 0;
    uint16_t i = start_checksum;
    uint16_t multiplier = 100;
    
    while(i <= end_checksum) {
        checksum = checksum + multiplier * (command[i] - '0');
        i++;
        multiplier /= 10;
    }
    
    uint16_t commandsum = 0;
    for(i = start_commandsum; i <= end_commandsum; i++) {
        commandsum += command[i];
        commandsum %= 256; // volta a 0 aos 255
    }

    if(commandsum == checksum) {
        return CHECKSUM_MATCH;
    }
    return CHECKSUM_MISMATCH;

}

void fifo_thread_code(void *argA , void *argB, void *argC) {
   
   struct uart_data_item_t *rx_data;

    while(1) {

        rx_data = k_fifo_get(&uart_fifo, K_FOREVER);

        if(rx_data != NULL) {


            /* Extract command from buffer */
            uint16_t command_len;

            if(rx_data->rx_buf_end >= rx_data->rx_buf_start) {
                command_len = (rx_data->rx_buf_end - rx_data->rx_buf_start + 1); 
            } else {
                command_len = (RXBUF_SIZE - rx_data->rx_buf_start + rx_data->rx_buf_end + 1); 
            }

            uint8_t *command = (uint8_t *)malloc(command_len + 1);

            for(int i = 0; i < command_len; i++) {
                command[i] = rx_data->rx_chars[(rx_data->rx_buf_start + i) % RXBUF_SIZE];
            }

            command[command_len] = 0; /* Terminate the string */

            printf("EXTRACTED COMMAND: %s\n", command);

            if(validate_command(command) == VALID_COMMAND && validate_checksum(command, command_len) == CHECKSUM_MATCH) {
                switch(command[1]) {
                    case 'B':
                        //get button status
                        break;
                    case 'L':
                        printf("***** CASE L\n");
                        if(command[3] == '1' || command[3] == '0') {
                            printf("***** LED CHANGED\n");
                            set_led(command[2]-'0', command[3]-'0');
                        }
                        //get or change led status
                        break;
                    case 'A':
                        //get raw or val adc value
                        break;
                    default:
                        printf("INVALID COMMAND!\n");
                        break;
                }
            }

            k_free(rx_data); 
        }

    }
}
