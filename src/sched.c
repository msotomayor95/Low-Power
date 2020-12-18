/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"


seed_t seed_array[MAX_SEEDS];
mr_meeseek_t meeseeks[20];
uint8_t ticks_reloj[20]; 				// Se usara para checkear si tengo que reducir la distancia maxima del meeseek.

uint32_t abs(int n){
	return n<0? -n:n;
}

uint8_t dist_manhattan(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
	int x = x2 - x1;
	int y = y2 - y1;
	
	return abs(x) + abs(y);
}

void sched_init(void) {
	cant_meeseeks_rick = 0;
	puntaje_rick = 0;
	
	cant_meeseeks_morty = 0;
	puntaje_morty = 0;

	tarea_actual = -1;
	for (int i = 0; i < MAX_SEEDS; ++i) {
		seed_array[i].x = (uint8_t)(rand() % 80);
		seed_array[i].y = (uint8_t)(rand() % 40);
		seed_array[i].found = 0;
		print("S",seed_array[i].x, seed_array[i].y + 1, C_FG_LIGHT_BROWN + C_BG_GREEN);
	}
	
	for (int i = 0; i < 20; ++i) {
		meeseeks[i].x = 255;
		meeseeks[i].y = 255;
		meeseeks[i].vivo = 0;
		meeseeks[i].dist_max = 0;
		meeseeks[i].uso_portal_gun = 0;

		ticks_reloj[i] = 0;
	}
}

uint16_t sched_next_task(void) {
	tarea_actual = (tarea_actual + 1) % 22;
	if (tarea_actual > 1) { // La siguiente tarea a ejecutar puede ser un Mr. Meeseek.
		int i = tarea_actual-2;
		while (i < 20){
			if (meeseeks[i].vivo == 1){
				break;
			}
			i++;
		}
		if (i < 20) {
			reducir_dist_maxima(i);
			tarea_actual = i+2;
		} else {
			tarea_actual = 0;
		}
	}
	uint16_t resultado = ((tarea_actual + FIRST_TSS) << 3);
	actualizar_pantalla();
	print_dec(tarea_actual, 2, 0, 0, 0xF);
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

uint8_t valores_validos(uint32_t code_start, uint32_t x, vaddr_t y) {

	uint8_t x_valido = x < 80 ? 1:0;
	uint8_t y_valido = y < 40 ? 1:0;
	uint8_t code_start_valido = code_start >= 0x1D00000 && code_start <= 0x1D02000 ? 1:0;

	if (x_valido == 1 && y_valido == 1 && code_start_valido == 1) {
		return 1;
	} else {
		return 0;
	}
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

void reducir_dist_maxima(uint8_t index) {
	ticks_reloj[index]--;

	if (ticks_reloj[index] == 0){
		if (meeseeks[index].dist_max > 1) {
			meeseeks[index].dist_max--;
		}
		ticks_reloj[index] = 2;
	}
}

void asimilar_semilla(uint8_t i){
	seed_array[i].found = 1;
	if (tarea_actual % 2 == 0) {
		puntaje_rick += 425;
	} else {
		puntaje_morty += 425;
	}
}

uint32_t crear_meeseek(vaddr_t code_start, uint8_t x, uint8_t y) {
	uint32_t i = tarea_actual == 0? 0 : 1;
	uint32_t j = 0;

	while (j < MAX_SEEDS && (seed_array[j].x != x || seed_array[j].y !=y)) j++;
	
	if (j < MAX_SEEDS && seed_array[j].found == 0) {
		asimilar_semilla(j);

		if(todas_las_semillas_encontradas() == 1) {
			sentenciar_ganador(0);
		}
		
		return 0;
	}

	while (i < 20 && meeseeks[i].vivo == 1) i+=2;
	
	meeseeks[i].vivo = 1;
	meeseeks[i].dist_max = 7;
	meeseeks[i].x = x;
	meeseeks[i].y = y;

	ticks_reloj[i] = 2;

	tarea_actual == 0? cant_meeseeks_rick++: cant_meeseeks_morty++;	
	uint32_t resultado = tss_task_init(i, code_start, x, y);
	return resultado;
}

uint8_t todas_las_semillas_encontradas() {
	uint8_t i = 0;
	while (i < MAX_SEEDS && seed_array[i].found == 1) i++;
	return i == MAX_SEEDS? 1:0;
}

void sentenciar_ganador(uint8_t por_excepcion) {
	if (por_excepcion == 0) {
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

	while(1) {
		__asm volatile("nop");
	}
}

void actualizar_pantalla(){
	// Base mapa.
	screen_draw_box (1, 0, 40, 80, 255, C_BG_GREEN);
	
	// puntuacion Rick
	// screen_draw_box (43, 5, 3, 10, 255, C_BG_RED);
	print_dec(puntaje_rick, 8, 6, 44, C_FG_WHITE + C_BG_RED);

	// puntuacion Morty
	// screen_draw_box (43, 65, 3, 10, 255, C_BG_BLUE);
	print_dec(puntaje_morty, 8, 66, 44, C_FG_WHITE + C_BG_BLUE);

	// Dibujo semillas no encontradas
	for (int i = 0; i < MAX_SEEDS; i++) {
		if (seed_array[i].found == 0) {
			print("S",seed_array[i].x, seed_array[i].y+1, C_FG_LIGHT_BROWN + C_BG_GREEN);
		}
	}

	// Dibujo meeseeks vivos
	for (int i = 0; i < 20; i++) {
		if (meeseeks[i].vivo == 1) {
			uint16_t color = i % 2 == 0? C_FG_RED: C_FG_BLUE;
			print("K",meeseeks[i].x, meeseeks[i].y+1, color + C_BG_GREEN);
		}
	}

}

uint32_t mover_meeseek(int x, int y) {

	uint8_t dist = abs(x) + abs(y);
	if (meeseeks[tarea_actual-2].dist_max < dist) {
		return 0;
	}

	int nuevo_x = (meeseeks[tarea_actual-2].x + x) % 80;
	int nuevo_y = (meeseeks[tarea_actual-2].y + y) % 40;

	if (nuevo_x < 0) {
		nuevo_x = 80 + nuevo_x;
	}

	if (nuevo_y < 0) {
		nuevo_y = 40 + nuevo_y;
	}

	int i = 0;
	while (i < MAX_SEEDS) {
		if (seed_array[i].found == 0 && seed_array[i].x == nuevo_x && seed_array[i].y == nuevo_y) {
			asimilar_semilla(i);
			matar_meeseek();

			if (todas_las_semillas_encontradas() == 1) {
				sentenciar_ganador(0);
			}
			return 1;
		}
		i++;
	}

	mover_codigo_meeseek(tarea_actual-2, nuevo_x, nuevo_y);
	meeseeks[tarea_actual-2].x = (uint8_t) nuevo_x;
	meeseeks[tarea_actual-2].y = (uint8_t) nuevo_y;
	return 1;
}

int semilla_x() {

	if (tarea_actual == 0 || tarea_actual == 1) {
		return -1;
	}

	uint8_t result_index = 0;
	uint8_t current_dist = 255;

	mr_meeseek_t m = meeseeks[tarea_actual - 2];

	for(int j = 0; j < MAX_SEEDS; j++) {
		uint8_t d = dist_manhattan(m.x, m.y, seed_array[j].x, seed_array[j].y);  
		if (d < current_dist ){
			current_dist = d;
			result_index = j;
		}
	}
	

	int x = seed_array[result_index].x - m.x;
	return x;
}

int semilla_y() {

	if (tarea_actual == 0 || tarea_actual == 1) {
		return -1;
	}

	uint8_t result_index = 0;
	uint8_t current_dist = 255;

	mr_meeseek_t m = meeseeks[tarea_actual - 2];

	for(int j = 0; j < MAX_SEEDS; j++) {
		uint8_t d = dist_manhattan(m.x, m.y, seed_array[j].x, seed_array[j].y);  
		if (d < current_dist ){
			current_dist = d;
			result_index = j;
		}
	}
	

	int y = seed_array[result_index].y - m.y;
	return y;
}

uint8_t ningun_meeseek_existente() {
	uint8_t i = 0;
	while (i < 20 && meeseeks[i].vivo == 0) i++;
	return i < 20? 0:1;
}

uint8_t uso_portal_gun() {
	return meeseeks[tarea_actual-2].uso_portal_gun;
}

void portal_gun() {
	uint8_t player_actual = tarea_actual % 2;
	
	if (ningun_meeseek_existente(player_actual) == 1) {
		meeseeks[tarea_actual-2].uso_portal_gun = 1;
		return;
	}

	uint8_t rn = player_actual == 0? 0:1;
	while(rn < 20) {
		if (meeseeks[rn].vivo == 1) break;
		rn += 2;
	}

	// uint8_t rn;
	// rn = rand() % 10;
	// rn = player_actual == 1? rn+1: rn;
	
	// while (meeseeks[rn].vivo == 0) {
	// 	rn = rand() % 10;
	// 	rn = player_actual == 1? rn+1: rn;
	// }


	uint8_t dist_max_backup = meeseeks[rn].dist_max;
	meeseeks[rn].dist_max = 255;
	mover_meeseek(rand() % 80, rand() % 40);
	meeseeks[rn].dist_max = dist_max_backup;
}