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
	uint8_t x = abs(x2 - x1);
	uint8_t y = abs(y2 - y1);
	
	return x + y;
}

void sched_init(void) {
	modo_debug = 1;
	modo_debug_corriendo = 0;

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
	if (tarea_actual > 1) {
		reducir_dist_maxima(tarea_actual-2); // Se reduce despues haber corrido al meeseek.
	}

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
			tarea_actual = i+2;
			// reducir_dist_maxima(i);
		} else {
			tarea_actual = 0;
		}
	}
	uint16_t resultado = ((tarea_actual + FIRST_TSS) << 3);
	actualizar_pantalla();
	// print_dec(tarea_actual, 2, 0, 0, 0xF);
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

	// print_dec(ticks_reloj[index], 1, 0, 0, 0xF);
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

	// print_dec(meeseeks[i].x, 2, 3, 0, 0xF);
	// print_dec(meeseeks[i].y, 2, 6, 0, 0xF);

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
	
	// print_dec(meeseeks[tarea_actual-2].x, 2, 3, 0, 0xF);
	// print_dec(meeseeks[tarea_actual-2].y, 2, 6, 0, 0xF);

	return 1;
}

int semilla_x() {

	if (tarea_actual == 0 || tarea_actual == 1) {
		// print("-1", 0, 0, 0xF);
		return -1;
	}

	uint8_t result_index = 0;
	uint8_t current_dist = 255;

	mr_meeseek_t m = meeseeks[tarea_actual - 2];

	for(int j = 0; j < MAX_SEEDS; j++) {
		if (seed_array[j].found == 0) {
			uint8_t d = dist_manhattan(m.x, m.y, seed_array[j].x, seed_array[j].y);  
			if (d < current_dist ){
				current_dist = d;
				result_index = j;
			}
		}
	}

	int x = seed_array[result_index].x - m.x;
	// print_dec(tarea_actual, 2, 0, 0, 0xF);
	// print_dec(x, 2, 3, 0, 0xF);

	return x;
}

int semilla_y() {

	if (tarea_actual == 0 || tarea_actual == 1) {
		// print("-1", 2, 0, 0xF);
		return -1;
	}

	uint8_t result_index = 0;
	uint8_t current_dist = 255;

	mr_meeseek_t m = meeseeks[tarea_actual - 2];

	for(int j = 0; j < MAX_SEEDS; j++) {
		if (seed_array[j].found == 0) {
			uint8_t d = dist_manhattan(m.x, m.y, seed_array[j].x, seed_array[j].y);  
			if (d < current_dist ){
				current_dist = d;
				result_index = j;
			}
		}
	}
	

	int y = seed_array[result_index].y - m.y;
	// print_dec(y, 2, 6, 0, 0xF);
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
	meeseeks[tarea_actual-2].uso_portal_gun = 1;
	uint8_t player_actual = tarea_actual % 2;
	
	if (ningun_meeseek_existente(player_actual) == 1) {
		return;
	}

	uint8_t rn;
	rn = rand() % 10;
	rn = player_actual == 1? rn: rn+1;
	
	while (meeseeks[rn].vivo == 0) {
		rn = rand() % 10;
		rn = player_actual == 1? rn: rn+1;
	}


	uint8_t dist_max_backup = meeseeks[rn].dist_max;
	meeseeks[rn].dist_max = 255;
	uint8_t x = rand() % 80;
	uint8_t y = rand() % 40;
	
	uint8_t backup_tarea_actual = tarea_actual;
	tarea_actual = rn+2;
	mover_meeseek(x, y);
	tarea_actual = backup_tarea_actual;

	meeseeks[rn].dist_max = dist_max_backup;
}

uint8_t check_modo_debug() {
	return modo_debug;
}

uint8_t check_modo_debug_corriendo() {
	return modo_debug_corriendo;
}

void cambiar_modo_debug() {
	if (modo_debug == 0 ) {
		modo_debug = 1;
		// print("modo debug activado", 0, 0, 0xF);
	} else {
		if (modo_debug_corriendo == 1) {
			modo_debug_corriendo = 0;
			// print("modo debug dejo de correr", 35, 0, 0xF);
		}
	}
}


char *excepName[] = {
	"0 Error de division",
	"1 Reservada",
	"2 NMI",
	"3 Breakpoint",
	"4 Overflow",
	"5 BOUND Range Exceeded",
	"6 Invalid Opcode",
	"7 CP no disponible",
	"8 Double fault",
	"9 CP Segment Overun",
	"10 TSS invalido",
	"11 Segmento no presente",
	"12 SS fault",
	"13 General Protection",
	"14 Page fault",
	"15 Reservada",
	"16 Error FPU",
	"17 Alignment Check",
	"18 Machine Check",
	"19 SIMD exception"
};


void mostrar_pantalla_debug(uint32_t gs,
							uint32_t fs,
							uint32_t es,
							uint32_t ds,
							uint32_t cr0, 
							uint32_t cr2, 
							uint32_t cr3, 
							uint32_t cr4,
							uint32_t exceptionIndex,
							uint32_t ss,
							uint32_t esp,
							uint32_t eflags,
							uint32_t cs,
							uint32_t eip,
							uint32_t errorCode,
							uint32_t edi,
							uint32_t esi, 
							uint32_t ebp, 
							uint32_t ignorar, //aca pushad me pushea el esp de nivel 0, no me importa
							uint32_t ebx,
							uint32_t edx,
							uint32_t ecx,
							uint32_t eax
							) {
	modo_debug_corriendo = 1;
	
	ignorar++;

	int32_t startX = 20;
	int32_t startY = 1;

	screen_draw_box(1, 19, 40, 42, 255, C_BG_BLACK);
	
	print(excepName[exceptionIndex], 21, 3, C_FG_LIGHT_GREEN);
	print    ("eax", startX+2, startY+ 5, C_FG_WHITE);
	print_hex(eax,8, startX+6, startY+ 5, C_FG_LIGHT_GREEN);

	print    ("ebx", startX+2, startY+ 7, C_FG_WHITE);
	print_hex(ebx,8, startX+6, startY+ 7, C_FG_LIGHT_GREEN);

	print    ("ecx", startX+2, startY+ 9, C_FG_WHITE);
	print_hex(ecx,8, startX+6, startY+ 9, C_FG_LIGHT_GREEN);

	print    ("edx", startX+2, startY+11, C_FG_WHITE);
	print_hex(edx,8, startX+6, startY+11, C_FG_LIGHT_GREEN);

	print    ("esi", startX+2, startY+13, C_FG_WHITE);
	print_hex(esi,8, startX+6, startY+13, C_FG_LIGHT_GREEN);

	print    ("edi", startX+2, startY+15, C_FG_WHITE);
	print_hex(edi,8, startX+6, startY+15, C_FG_LIGHT_GREEN);

	print    ("ebp", startX+2, startY+17, C_FG_WHITE);
	print_hex(ebp,8, startX+6, startY+17, C_FG_LIGHT_GREEN);

    print    ("esp", startX+2, startY+19, C_FG_WHITE);
    print_hex(esp, 8, startX+6, startY+19, C_FG_LIGHT_GREEN);

    print    ("eip", startX+2, startY+21, C_FG_WHITE);
    print_hex(eip, 8, startX+6, startY+21, C_FG_LIGHT_GREEN);

    print    ("cs", startX+3, startY+23, C_FG_WHITE);
    print_hex(cs, 8, startX+6, startY+23, C_FG_LIGHT_GREEN);

    print    ("ds", startX+3, startY+25, C_FG_WHITE);
    print_hex(ds, 8, startX+6, startY+25, C_FG_LIGHT_GREEN);

    print    ("es", startX+3, startY+27, C_FG_WHITE);
    print_hex(es, 8, startX+6, startY+27, C_FG_LIGHT_GREEN);
  
    print    ("fs", startX+3, startY+29, C_FG_WHITE);
    print_hex(fs, 8, startX+6, startY+29, C_FG_LIGHT_GREEN);

    print    ("gs", startX+3, startY+31, C_FG_WHITE);
    print_hex(gs, 8, startX+6, startY+31, C_FG_LIGHT_GREEN);

	print    ("ss", startX+3, startY+33, C_FG_WHITE);
	print_hex(ss, 8, startX+6, startY+33, C_FG_LIGHT_GREEN);

	print    ("eflags", startX+2, startY+37, C_FG_WHITE);
    print_hex(eflags, 8, startX+9, startY+37, C_FG_LIGHT_GREEN);

	print    ("cr0", startX+28, startY+ 6, C_FG_WHITE);
	print_hex(cr0,8, startX+32, startY+ 6, C_FG_LIGHT_GREEN);

	print    ("cr2", startX+28, startY+ 8, C_FG_WHITE);
	print_hex(cr2,8, startX+32, startY+ 8, C_FG_LIGHT_GREEN);	

	print    ("cr3", startX+28, startY+10, C_FG_WHITE);
	print_hex(cr3,8, startX+32, startY+10, C_FG_LIGHT_GREEN);

	print    ("cr4", startX+28, startY+12, C_FG_WHITE);
	print_hex(cr4,8, startX+32, startY+12, C_FG_LIGHT_GREEN);

	print    ("err", startX+28, startY+14, C_FG_WHITE);
    print_hex(errorCode, 8, startX+32, startY+14, C_FG_LIGHT_GREEN);


    uint32_t* p = (uint32_t *) (esp-12);

    print ("stack", startX+22, startY+18, C_FG_WHITE);

	print_hex(p[0], 8, startX+22, startY+20, C_FG_LIGHT_GREEN);
	print_hex(p[1], 8, startX+22, startY+22, C_FG_LIGHT_GREEN);
	print_hex(p[2], 8, startX+22, startY+24, C_FG_LIGHT_GREEN);

	print ("backtrace", startX+22, startY+26, C_FG_WHITE);

	p = (uint32_t *) (ebp);
	
	print_hex(0x0, 8, startX+22, startY+28, C_FG_LIGHT_GREEN);
	print_hex(0x0, 8, startX+22, startY+30, C_FG_LIGHT_GREEN);
	print_hex(0x0, 8, startX+22, startY+32, C_FG_LIGHT_GREEN);
	print_hex(0x0, 8, startX+22, startY+34, C_FG_LIGHT_GREEN);

	if (p[1] != 0 && (0x8000000 < p[1] && p[1] < 0x8014000)) {
		print_hex((uint32_t) p[1], 8, startX+22, startY+28, C_FG_LIGHT_GREEN);
		p = (uint32_t *) p[0];
		if (p[1] != 0 && (0x8000000 < p[1] && p[1] < 0x8014000)) {
			print_hex((uint32_t) p[1], 8, startX+22, startY+30, C_FG_LIGHT_GREEN);
			p = (uint32_t *) p[0];
			if (p[1] != 0 && (0x8000000 < p[1] && p[1] < 0x8014000)) {
				print_hex((uint32_t) p[1], 8, startX+22, startY+32, C_FG_LIGHT_GREEN);
				p = (uint32_t *) p[0];
				if (p[1] != 0 && (0x8000000 < p[1] && p[1] < 0x8014000)) {
					print_hex((uint32_t) p[1], 8, startX+22, startY+34, C_FG_LIGHT_GREEN);
					p = (uint32_t *) p[0];
				}
			}
		}

	} 


	// for (int i = 0; i < 4; ++i) {
		
	// }

	while (modo_debug_corriendo == 1){
		__asm volatile("nop");
	}

	actualizar_pantalla(); // restauro la pantalla 
}

uint8_t present_error_code(uint32_t exceptionIndex){
	uint8_t res = 0;
	if (exceptionIndex == 8 || ((exceptionIndex >= 10) && (exceptionIndex <= 14)) || exceptionIndex == 17 || exceptionIndex == 30) {		// PERDON 
		res = 1;
	}
	return res;
}