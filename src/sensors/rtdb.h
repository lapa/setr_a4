
/*
 * Este ficheiro serve para integrar os pedidos de leitura da UART com as atualizações vindas da placa
 *
 */

#include "adc.h"
#include "leds.h"
#include "buttons.h"
#include <zephyr/kernel.h>


//Vamos ter que definir os mutexes e as estruturas onde os dados estarão guardados

int leds[4];
int buttons[4];
int adc_raw;
int adc_an_val;

struct k_mutex leds_mutex[4];
struct k_mutex buttons_mutex[4];
struct k_mutex adc_raw_mutex;
struct k_mutex adc_an_mutex;


void rtdb_read_adc_raw(int *res);
void rtdb_read_adc_an_val(int *res);
void rtdb_read_led(int id, int *res);
void rtdb_read_button(int id, int *res);

void rtdb_set_adc_raw(int value);
void rtdb_set_adc_an(int value);
void rtdb_set_led(int id, int value);
void rtdb_set_button(int id, int value);
