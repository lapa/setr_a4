
#include "rtdb.h"


int leds[4];
int buttons[4];
int adc_raw;
int adc_an_val;

struct k_mutex leds_mutex[4];
struct k_mutex buttons_mutex[4];
struct k_mutex adc_raw_mutex;
struct k_mutex adc_an_mutex;

void rtdb_read_adc_raw(int *res) {
	k_mutex_lock(&adc_raw_mutex, K_FOREVER);
	*res = adc_raw;
	k_mutex_unlock(&adc_raw_mutex);
}

void rtdb_read_adc_an(int *res) {
	k_mutex_lock(&adc_an_mutex, K_FOREVER);
	*res = adc_an_val;
	k_mutex_unlock(&adc_an_mutex);
}

void rtdb_read_led(int id, int *res) {
	k_mutex_lock(&leds_mutex[id], K_FOREVER);
	*res = leds[id];
	k_mutex_unlock(&leds_mutex[id]);
}

void rtdb_read_button(int id, int *res) {
	k_mutex_lock(&buttons_mutex[id], K_FOREVER);
	*res = buttons[id];
	k_mutex_unlock(&buttons_mutex[id]);
}

void rtdb_set_adc_raw(int value) {
	k_mutex_lock(&adc_raw_mutex, K_FOREVER);
	adc_raw = value;
	k_mutex_unlock(&adc_raw_mutex);
}

void rtdb_set_adc_an_val(int value) {
	k_mutex_lock(&adc_an_mutex, K_FOREVER);
	adc_an_val = value;
	k_mutex_unlock(&adc_an_mutex);
}
void rtdb_set_led(int id, int value) {
	k_mutex_lock(&leds_mutex[id], K_FOREVER);
	leds[id] = value;
	k_mutex_unlock(&leds_mutex[id]);
}

void rtdb_set_button(int id, int value) {
	k_mutex_lock(&buttons_mutex[id], K_FOREVER);
	buttons[id] = value;
	k_mutex_unlock(&buttons_mutex[id]);
}



