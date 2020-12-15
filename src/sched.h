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
} mr_meeseek_t;


seed_t seed_array[MAX_SEEDS];
mr_meeseek_t meeseeks[20];

int tarea_actual;
int puntaje_rick;
int puntaje_morty;
int cant_meeseeks_rick;
int cant_meeseeks_morty;

void sched_init();
uint16_t sched_next_task();


uint8_t meeseek_llamo_crear_meeseek();
uint8_t valores_validos();
uint8_t puedo_crear_meeseek();
uint32_t crear_meeseek(uint8_t x, uint8_t y, uint32_t code_start);
uint8_t todas_las_semillas_encontradas();
void sentenciar_ganador(uint8_t por_excepcion);
void actualizar_pantalla();
void matar_meeseek();

#endif //  __SCHED_H__
