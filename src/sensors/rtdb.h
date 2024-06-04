
/*
 * Este ficheiro serve para integrar os pedidos de leitura da UART com as atualizações vindas da placa
 *
 */

#ifndef __RTDB_H__
#define __RTDB_H__

#include <zephyr/kernel.h>

//Vamos ter que definir os mutexes e as estruturas onde os dados estarão guardados

void rtdb_read_adc_raw(int *res);
void rtdb_read_adc_an_val(int *res);
void rtdb_read_led(int id, int *res);
void rtdb_read_button(int id, int *res);

void rtdb_set_adc_raw(int value);
void rtdb_set_adc_an(int value);
void rtdb_set_led(int id, int value);
void rtdb_set_button(int id, int value);

#endif
