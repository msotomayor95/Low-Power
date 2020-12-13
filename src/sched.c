/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"


void sched_init(void) {
	cant_meeseeks_rick = 0;
	cant_meeseeks_morty = 0;

	tarea_actual = 0;
	for (int i = 0; i < 11; ++i) {
		seed_array[i].x = (uint8_t)(rand() % 80);
		seed_array[i].y = (uint8_t)(rand() % 40);
		screen_draw_box (seed_array[i].y, seed_array[i].x, 1, 1, 83, C_FG_BROWN + C_BG_GREEN);
	}
	
	for (int i = 0; i < 20; ++i) {
		meeseeks[i].vivo = 0;
	}
}

uint16_t sched_next_task(void) {
  tarea_actual = (tarea_actual + 1) % 2;
  uint16_t resultado = ((tarea_actual + FIRST_TSS) << 3);
  return resultado;
}

uint8_t puedo_crear_meeseek() {
	if (tarea_actual == 0 && cant_meeseeks_rick < 10) {
		return 1;
	} else if (tarea_actual == 1 && cant_meeseeks_morty < 10) {
		return 1;
	} else {
		return 0;
	}
}

uint32_t crear_meeseek(uint8_t x, uint8_t y, vaddr_t code_start) {
	uint32_t i = tarea_actual == 0? 0 : 1;
	while (i < 20 && meeseeks[i].vivo == 1) i+=2;

	meeseeks[i].vivo = 1;
	meeseeks[i].x = x;
	meeseeks[i].y = y;

	

	tss_task_init(i, code_start, x, y);
}