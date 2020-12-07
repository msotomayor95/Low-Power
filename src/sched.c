/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"



void sched_init(void) {
	tarea_actual = 0;

}

uint16_t sched_next_task(void) {
  tarea_actual = (tarea_actual + 1) % 2;
  uint16_t resultado = ((tarea_actual + FIRST_TSS) << 3);
  return resultado;
}