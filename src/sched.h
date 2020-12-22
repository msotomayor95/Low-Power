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
#include "tss.h"
#include "colors.h"

typedef struct megaSemilla
{
	uint8_t x;
	uint8_t y;
	uint8_t found;
} seed_t;

typedef struct meeseek
{
	uint8_t x;
	uint8_t y;
	uint8_t vivo;
	uint8_t dist_max;
	uint8_t uso_portal_gun;
} mr_meeseek_t;

uint8_t modo_debug;
uint8_t modo_debug_corriendo;
int tarea_actual;
int puntaje_rick;
int puntaje_morty;
int cant_meeseeks_rick;
int cant_meeseeks_morty;

void sched_init();
uint16_t sched_next_task();


uint8_t meeseek_llamo_crear_meeseek();
uint8_t valores_validos(uint32_t code_start, uint32_t x, vaddr_t y);
uint8_t puedo_crear_meeseek();
uint32_t crear_meeseek(vaddr_t code_start, uint8_t x, uint8_t y);
uint32_t mover_meeseek(int x, int y);
void reducir_dist_maxima(uint8_t index);
void asimilar_semilla(uint8_t s);
uint8_t todas_las_semillas_encontradas();
uint8_t uso_portal_gun();
uint8_t ningun_meekseek_existente(uint8_t index);
void portal_gun();
void sentenciar_ganador(uint8_t por_excepcion);
int semilla_x();
int semilla_y();
void actualizar_pantalla();
void matar_meeseek();

uint8_t check_modo_debug();
uint8_t check_modo_debug_corriendo();

void cambiar_modo_debug();
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
							);
uint8_t present_error_code(uint32_t exceptionIndex);


#endif //  __SCHED_H__
