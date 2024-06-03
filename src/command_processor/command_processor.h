#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

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

uint16_t validate_command(char *command);
uint16_t validate_checksum(char *command, uint16_t rx_occupied_bytes);

#endif