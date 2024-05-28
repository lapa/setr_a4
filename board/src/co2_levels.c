
#include "co2_levels.h"

uint16_t co2levels[SENSOR_CACHE_SIZE+1];
uint16_t co2levels_last = 0;
uint16_t more_20_co2s = 0;
uint16_t all_co2_levels[NUM_SAMPLES] = { 7524, 6942, 17760, 11267, 14115, 13289,
	13410, 2896, 18347, 4499, 19297, 5312, 2175, 18053, 6888, 671, 14137, 19925,
	18978, 13048, 19773, 13704, 7337, 14087, 5248, 19812, 4111, 13252, 6610, 5159,
	4132, 9233, 12526, 17417, 18090, 6902, 19685, 7580, 12508, 428};
uint16_t co2_sample = 0;

uint16_t get_co2_history(uint16_t index, uint16_t *value) {
	if (index <= SENSOR_CACHE_SIZE && index) {
		*value = co2levels[index];
		return SUCCESS;
	}
	return VALUE_NOT_AVAILABLE;
}


uint16_t read_sensor_co2(uint16_t *value) {
	generate_co2();
	*value = co2levels[co2levels_last];
	return SUCCESS;
}

uint16_t generate_co2() {
	co2levels[++co2levels_last] = all_co2_levels[co2_sample++];
	co2_sample %= NUM_SAMPLES;
	co2levels_last %= SENSOR_CACHE_SIZE;
	if (co2levels_last == SENSOR_CACHE_SIZE - 1) more_20_co2s = 1;
	return SUCCESS;
}

uint16_t check_co2levels() { 
	return more_20_co2s;
}

uint16_t reset_co2levels() {
	co2levels_last = 0;
	more_20_co2s = 0;
	return SUCCESS;	
}
