/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"

tss_t tss_initial = {0};
tss_t tss_idle = {0};

void tss_init(void) {
	tss_idle.ptl = 0;
  	tss_idle.unused0 = 0;
	tss_idle.esp0 = 0;
	tss_idle.ss0 = 0;
	tss_idle.unused1 = 0;
	tss_idle.esp1 = 0;
	tss_idle.ss1 = 0;
	tss_idle.unused2 = 0;
	tss_idle.esp2 = 0;
	tss_idle.ss2 = 0;
	tss_idle.unused3 = 0;
	tss_idle.cr3 = 0x25000;
	tss_idle.eip = 0x18000;
	tss_idle.eflags = 0x202;  // cambiar a 202
	tss_idle.eax = 0;
	tss_idle.ecx = 0;
	tss_idle.edx = 0;
	tss_idle.ebx = 0;
	tss_idle.esp = 0x25000;
	tss_idle.ebp = 0x25000;
	tss_idle.esi = 0;
	tss_idle.edi = 0;
	tss_idle.es = GDT_IDX_DATA_0<<3;
	tss_idle.unused4 = 0;
	tss_idle.cs = GDT_IDX_CODE_0<<3;
	tss_idle.unused5 = 0;
	tss_idle.ss = GDT_IDX_DATA_0<<3;
	tss_idle.unused6 = 0;
	tss_idle.ds = GDT_IDX_DATA_0<<3;
	tss_idle.unused7 = 0;
	tss_idle.fs = GDT_IDX_DATA_0<<3;
	tss_idle.unused8 = 0;
	tss_idle.gs = GDT_IDX_DATA_0<<3;
	tss_idle.unused9 = 0;
	tss_idle.ldt = 0;
	tss_idle.unused10 = 0;
	tss_idle.dtrap = 0;
	tss_idle.iomap = 0;

	gdt[GDT_IDX_TSS_INIT].base_15_0 = (uint16_t)((uint32_t)&tss_initial);
	gdt[GDT_IDX_TSS_INIT].base_23_16 = (uint8_t)(((uint32_t)&tss_initial)>>16);
	gdt[GDT_IDX_TSS_INIT].base_31_24 = (uint8_t)(((uint32_t)&tss_initial)>>24);

	gdt[GDT_IDX_TSS_IDLE].base_15_0 = (uint16_t)((uint32_t)&tss_idle);
	gdt[GDT_IDX_TSS_IDLE].base_23_16 = (uint8_t)(((uint32_t)&tss_idle)>>16);
	gdt[GDT_IDX_TSS_IDLE].base_31_24 = (uint8_t)(((uint32_t)&tss_idle)>>24);
}