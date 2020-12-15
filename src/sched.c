/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"


void sched_init(void) {
	cant_meeseeks_rick = 0;
	puntaje_rick = 0;
	
	cant_meeseeks_morty = 0;
	puntaje_morty = 0;

	tarea_actual = 0;
	for (int i = 0; i < MAX_SEEDS; ++i) {
		seed_array[i].x = (uint8_t)(rand() % 80);
		seed_array[i].y = (uint8_t)(rand() % 40);
		seed_array[i].found = 0;
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

uint8_t meeseek_llamo_crear_meeseek() {
	return tarea_actual > 1? 1 : 0;
}

void matar_meeseek() {
	uint32_t actual = tarea_actual - 2;
	meeseeks[actual].vivo = 0;

	actual % 2 == 0? cant_meeseeks_rick-- : cant_meeseeks_morty--;
	tss_task_kill(actual);
}

uint8_t valores_validos(uint32_t x, uint32_t y, vaddr_t code_start) {
	return (x < 80 && y < 50 && code_start >= 0x1D00000 && code_start <= 0x1D02000 )? 1 : 0;
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
	uint32_t j = 0;

	while (j < MAX_SEEDS && (seed_array[j].x != x || seed_array[j].y !=y)) j++;
	
	if (j < MAX_SEEDS && seed_array[j].found == 0) {
		if (tarea_actual == 0) {
			puntaje_rick +=5;
		} else {
			puntaje_morty+=5;
		}
		seed_array[j].found = 1;

		if(todas_las_semillas_encontradas() == 1) {
			sentenciar_ganador(0);
			while(1){
				__asm volatile("nop");
			};
		}
		
		return 0;
	}

	while (i < 20 && meeseeks[i].vivo == 1) i+=2;
	
	meeseeks[i].vivo = 1;
	meeseeks[i].x = x;
	meeseeks[i].y = y;

	tarea_actual == 0? cant_meeseeks_rick++: cant_meeseeks_morty++;	
	return tss_task_init(i, code_start, x, y);
}

uint8_t todas_las_semillas_encontradas() {
	uint8_t i = 0;
	while (i < MAX_SEEDS && seed_array[i].found == 1) i++;
	return i == MAX_SEEDS? 1:0;
}

void sentenciar_ganador(uint8_t por_excepcion) {
	if (por_excepcion == 0){
		if(puntaje_rick < puntaje_morty) {
			print("Gano Morty", 0, 0, 0xF);
		} else if ( puntaje_morty < puntaje_rick) {
			print("Gano Rick", 0, 0, 0xF);
		} else {
			print("Empate", 0, 0, 0xF);
		}
	} else {
		char* win_text = tarea_actual == 0? "Gano Morty":"Gano Rick";
		print(win_text, 0, 0, 0xF);
	}
}
