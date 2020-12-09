/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"



void sched_init(void) {
	tarea_actual = 0;
	for (int i = 0; i < 11; ++i) {
		seed_array[i].x = (uint8_t)(rand() % 80);
		seed_array[i].y = (uint8_t)(rand() % 40);
		print ("s", seed_array[i].x + 1, seed_array[i].y, C_FG_LIGHT_MAGENTA);
	}
}

uint16_t sched_next_task(void) {
  tarea_actual = (tarea_actual + 1) % 2;
  uint16_t resultado = ((tarea_actual + FIRST_TSS) << 3);
  return resultado;
}