#include "UART.h"

static unsigned char RxBuffer[BUFFER_SIZE];
static uint16_t rx_occupied_bytes = 0;
static unsigned char TxBuffer[BUFFER_SIZE];
static uint16_t tx_occupied_bytes = 0;
static regex_t regex;
static char* command_pattern = "^#(A|P[THC]|L|L[THC]|R|R[THC])(2[5][0-5]|2[0-4][0-9]|[0-1][0-9]{2})!$";

uint16_t check_sensor_identifier(unsigned char c) {
	return (c == 'T') || (c == 'H') || (c == 'C');
}

uint16_t receive_byte(unsigned char input) {

    if(rx_occupied_bytes < BUFFER_SIZE) {

        if(input == SOF_SYM) {
            clear_rx_buffer();
        }
        RxBuffer[rx_occupied_bytes] = input;
        rx_occupied_bytes++;

        if(input == EOF_SYM) {
            //get command in buffer
            char command[rx_occupied_bytes+1];
            strncpy(command, RxBuffer, rx_occupied_bytes);
            command[rx_occupied_bytes] = '\0'; //null-terminate the string;
            if(validate_command(command)==0 && validate_checksum(command, rx_occupied_bytes)==0) {
                switch(command[1]) {
                    case 'A':
			            return read_value_sensor_all();
                        break;
                    case 'P':
			printf("SENSORE:%d with %c\n", check_sensor_identifier(command[2]), command[2]);
                        if(!check_sensor_identifier(command[2])) {
				printf("\t\tFALHOW\n");
                            return UNKNOWN_SENSOR;
                        }
			            return read_value_sensor(command[2]);
                        break;
                    case 'L':
                        if(!check_sensor_identifier(command[2])) return send_last_20_samples_all();
                        else return send_last_20_samples(command[2]);
                        break;
                    case 'R':
                        if(!check_sensor_identifier(command[2])) return reset_history(command[2]);
                        else return reset_history_all();
                        break;
                    default:
                        return INVALID_COMMAND;
                }
                clear_rx_buffer();
                return FULL_COMMAND_RECEIVED;
            }
            clear_rx_buffer();
            return INVALID_COMMAND;
        }
        return BYTE_ADDED_TO_BUFFER;
    }
    clear_rx_buffer();
    return receive_byte(input);
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

uint16_t calculate_checksum() {
	uint16_t sum = 0;
	for(uint16_t i = 1; i < tx_occupied_bytes; i++) {
		sum = (sum + TxBuffer[i]) % 256;
	}
	return sum;
}

uint16_t send_byte(unsigned char input) {

    if(tx_occupied_bytes < BUFFER_SIZE) {

        if(input == SOF_SYM) {
            clear_tx_buffer();
        }
        TxBuffer[tx_occupied_bytes] = input;
        tx_occupied_bytes++;
        if(input == EOF_SYM) {
            //get command in buffer
            char command[tx_occupied_bytes+1];
            strncpy(command, TxBuffer, tx_occupied_bytes);
            command[tx_occupied_bytes] = '\0'; //null-terminate the string;
            printf("Full command sent: %s\n", command);
            return FULL_COMMAND_SENT;
        }
        return BYTE_ADDED_TO_BUFFER;
    }
    clear_tx_buffer();
    return send_byte(input);
}

void clear_rx_buffer() {
    rx_occupied_bytes = 0;
    return;
}

void clear_tx_buffer() {
    tx_occupied_bytes = 0;
    return;
}

unsigned char *returnRxBuffer(unsigned char *buffer) {
        strncpy(buffer, RxBuffer, rx_occupied_bytes);
        buffer[rx_occupied_bytes] = '\0'; //null-terminate the string;
	return buffer;
}

unsigned char *returnTxBuffer(unsigned char *buffer) {
        strncpy(buffer, TxBuffer, tx_occupied_bytes);
        buffer[tx_occupied_bytes] = '\0'; //null-terminate the string;
	return buffer;
}

/*
 * Execução dos comandos
 * */

uint16_t read_value_sensor_all() {

	send_byte('#');
	send_byte('A');
	send_byte('R');
	//Sends values by order (temperature, humidity, CO2)
	uint16_t value = 0;
	read_sensor_temp(&value);
	send_temp(value);
	send_byte('>');
	read_sensor_humidity(&value);
	send_humidity(value);
	send_byte('>');
	read_sensor_co2(&value);
	send_co2(value);
	uint16_t checksum = calculate_checksum();
	send_byte('|');
	if (checksum < 100) send_byte('0');
	if (checksum < 10) send_byte('0');
	send_number(checksum);
	send_byte('!');

	return SUCCESS;
}

uint16_t read_value_sensor(unsigned char sensor) {
	uint16_t value = 0;
	
	if (sensor == 'T') {
		send_byte('#');
		send_byte('P');
		send_byte('T');
		read_sensor_temp(&value);
		send_temp(value);
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
		return SUCCESS;
	}
	if (sensor == 'H') {
		send_byte('#');
		send_byte('P');
		send_byte('H');
		read_sensor_humidity(&value);
		send_humidity(value);
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
		return SUCCESS;
	}
	if (sensor == 'C') {
		send_byte('#');
		send_byte('P');
		send_byte('C');
		read_sensor_co2(&value);
		send_co2(value);
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
		return SUCCESS;
	}
	return NO_SENSOR;
}

uint16_t send_last_20_samples(unsigned char sensor) {
	
	uint16_t *samples;
	uint16_t more_20 = 0;
	if (sensor == 'T') {
		if (!check_temps()) return NOT_TWENTY_ENTRIES;
		send_byte('#');
		send_byte('P');
		send_byte('R');
		send_last_20_temps();
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
	} else if (sensor == 'H') {
		if (!check_humidities()) return NOT_TWENTY_ENTRIES;
		send_byte('#');
		send_byte('P');
		send_byte('R');
		send_last_20_humidities();
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
	} else if (sensor == 'C') {
		if (!check_co2levels()) return NOT_TWENTY_ENTRIES;
		send_byte('#');
		send_byte('P');
		send_byte('R');
		send_last_20_co2levels();
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
	} else return NO_SENSOR;
	
	return SUCCESS;
}

uint16_t send_last_20_samples_all() {

	if (check_temps() && check_humidities() && check_co2levels()) {
		send_byte('#');
		send_byte('L');
		send_byte('R');
		send_last_20_temps();
		send_byte('>');
		send_last_20_humidities();
		send_byte('>');
		send_last_20_co2levels();
		uint16_t checksum = calculate_checksum();
		send_byte('|');
		if (checksum < 100) send_byte('0');
		if (checksum < 10) send_byte('0');
		send_number(checksum);
		send_byte('!');
		return SUCCESS;
	}
	printf("FAILURE: %d %d %d\n", check_temps(), check_humidities(), check_co2levels());
	return NOT_TWENTY_ENTRIES;
}

uint16_t reset_history(unsigned char sensor) {
	if (sensor == 'T') {
		reset_temps();
	} else if (sensor == 'H') {
		reset_humidities();
	} else if (sensor == 'C') {
		reset_co2levels();
	} else return NO_SENSOR;
	return SUCCESS;
}


uint16_t reset_history_all() {
	reset_temps();
	reset_humidities();
	reset_co2levels();
	return SUCCESS;
}


/*
 * Send number
 * */

uint16_t send_number(uint16_t number) {
	
	if (!number) { //Verificar se o número não é zero
		send_byte('0');
		return SUCCESS;
	}

	uint16_t i = 10000; //Para a div_int o máximo que precisaremos é 10k

	while (number / i == 0) i /= 10; //Caso 10k seka muito grande, adequamos
	do {
		send_byte((unsigned char) (number / i) + '0');
		number %= i;
	} while(i=i/10) ;  //Fazemos até chegarmos à casa das unidades

	return SUCCESS;
}

/*
 * Module to send the temperature
 * */

uint16_t send_temp(uint16_t value_temp) {
	if (value_temp <= 50) {
		send_byte('-');
		value_temp = 50 - value_temp;
		send_number(value_temp);
	} else {
		send_byte('+');
		value_temp = value_temp - 50;
		send_number(value_temp);
	}
	return SUCCESS;
}

uint16_t send_last_20_temps() {
	if (!check_temps()) {
		return NOT_TWENTY_ENTRIES;
	}
	uint16_t value = 0;
	for (uint16_t i = 0; i < 20; i++) {
		get_temperature_history(i, &value);
		send_temp(value);
	}
	return SUCCESS;
}


/*
 * Module to send the temperature
 * */

uint16_t send_humidity(uint16_t value_humidity) {
	send_byte('+');
	send_number(value_humidity);
	return SUCCESS;
}

uint16_t send_last_20_humidities() {
	if (!check_humidities()) {
		return NOT_TWENTY_ENTRIES;
	}
	uint16_t value = 0;
	for (uint16_t i = 0; i < 20; i++) {
		get_humidity_history(i, &value);
		send_humidity(value);
	}
	return SUCCESS;
}

/*
 * Module to send co2
 * */

uint16_t send_co2(uint16_t value_co2) {
	send_byte('+');
	send_number(value_co2);
	return SUCCESS;
}

uint16_t send_last_20_co2levels() {
	if (!check_co2levels()) {
		return NOT_TWENTY_ENTRIES;
	}
	uint16_t value = 0;
	for (uint16_t i = 0; i < 20; i++) {
		get_co2_history(i, &value);
		send_co2(value);
	}
	return SUCCESS;
}







