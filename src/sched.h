/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "defines.h"
#include "prng.h"
#include "screen.h"
#include "colors.h"

typedef struct megaSemilla
{
	uint8_t x;
	uint8_t y;
} seed_t;

typedef struct meeseek
{
	uint8_t x;
	uint8_t y;
	uint8_t vivo;
} mr_meeseek_t;


seed_t seed_array[11];
mr_meeseek_t meeseeks[20];

int tarea_actual;
int cant_meeseeks_rick;
int cant_meeseeks_morty;

void sched_init();
uint16_t sched_next_task();

uint8_t puedo_crear_meeseek();
uint32_t crear_meeseek(uint8_t x, uint8_t y, uint32_t code_start);

#endif //  __SCHED_H__
