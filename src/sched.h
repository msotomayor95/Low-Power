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

seed_t seed_array[11];
int tarea_actual;
void sched_init();
uint16_t sched_next_task();

#endif //  __SCHED_H__
