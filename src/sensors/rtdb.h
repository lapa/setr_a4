
/*
 * Este ficheiro serve para integrar os pedidos de leitura da UART com as atualizações vindas da placa
 *
 */

#include "adc.h"
#include "leds.h"
#include "buttons.h"

//Vamos ter que definir os mutexes e as estruturas onde os dados estarão guardados

int leds[4];
int buttons[4];
int adc_raw;
int adc_an_val;

void read_adc_raw(int *res);
void read_adc_an_val(int *res);
void read_led(int id, int *res);
void read_button(int id, int *res);

void set_adc_raw(int value);
void set_adc_an(int value);
void set_led(int id, int value);
