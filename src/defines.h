/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__
/* MMU */
/* -------------------------------------------------------------------------- */

#define MMU_P (1 << 0)
#define MMU_W (1 << 1)
#define MMU_U (1 << 2)

#define PAGE_SIZE 4096

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40

// X Columnas
#define SIZE_M 80

//tamanio de una tss
#define SIZE_TSS 0x68

//first tss
#define FIRST_TSS 17

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC  0
#define GDT_IDX_DATA_0     10
#define GDT_IDX_DATA_3     11
#define GDT_IDX_CODE_0     12
#define GDT_IDX_CODE_3     13
#define GDT_IDX_VIDEO_0    14
#define GDT_IDX_TSS_INIT   15
#define GDT_IDX_TSS_IDLE   16
#define GDT_IDX_TSS_RICK   17
#define GDT_IDX_TSS_MORTY  18

#define GDT_COUNT          35


/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */

// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x1D00000
#define TASK_PAGES        4

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<X>_PHY_START.
 */

#define CODE_RICK     0x10000
#define CODE_MORTY 	  0x14000
#define PLAYER_RICK   0x1D00000
#define PLAYER_MORTY  0x1D04000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

#define FREE_KERNEL_SPACE (0x100000)

#define MMU_FLAG_PRESENT (0x1)
#define MMU_FLAG_SUPERVISOR (0x0)
#define MMU_FLAG_READWRITE (0x1)

#endif //  __DEFINES_H__
