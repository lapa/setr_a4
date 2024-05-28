
#include "temperature.h"

uint16_t temps[SENSOR_CACHE_SIZE+1];
uint16_t temp_last = 0;
uint16_t more_20_temps = 0;
uint16_t all_temperatures[NUM_SAMPLES] = { 85, 21, 48, 104, 81, 82, 8, 35, 74, 59, 73,
	40, 50, 64, 32, 23, 57, 10, 86, 7, 72, 95, 27, 105, 29, 61, 62, 38, 96, 25, 88,
	33, 15, 18, 46, 51, 26, 45, 3, 5} ;
uint16_t temperature_sample = 0;

uint16_t get_temperature_history(uint16_t index, uint16_t *value) {
	if (index <= SENSOR_CACHE_SIZE && index) {
		*value = temps[index];
		return SUCCESS;
	}
	return VALUE_NOT_AVAILABLE;
}

uint16_t read_sensor_temp(uint16_t *value) {
	generate_temp();
	*value = temps[temp_last];
	return SUCCESS;
}

uint16_t generate_temp() {
	temps[++temp_last] = all_temperatures[temperature_sample++];
	temperature_sample %= NUM_SAMPLES;
	temp_last %= SENSOR_CACHE_SIZE;
	if (temp_last == SENSOR_CACHE_SIZE - 1) more_20_temps = 1;
	return SUCCESS;
}
uint16_t check_temps() {
	return more_20_temps;
}

uint16_t reset_temps() {
	temp_last = 0;
	more_20_temps = 0;
	return SUCCESS;
}

