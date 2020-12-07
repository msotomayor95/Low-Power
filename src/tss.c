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
tss_t tss_idle = {
	.ptl = 0,
  	.unused0 = 0,
	.esp0 = 0,
	.ss0 = 0,
	.unused1 = 0,
	.esp1 = 0,
	.ss1 = 0,
	.unused2 = 0,
	.esp2 = 0,
	.ss2 = 0,
	.unused3 = 0,
	.cr3 = 0x25000,
	.eip = 0x18000,
	.eflags = 0x202,
	.eax = 0,
	.ecx = 0,
	.edx = 0,
	.ebx = 0,
	.esp = 0x25000,
	.ebp = 0x25000,
	.esi = 0,
	.edi = 0,
	.es = GDT_IDX_DATA_0<<3,
	.unused4 = 0,
	.cs = GDT_IDX_CODE_0<<3,
	.unused5 = 0,
	.ss = GDT_IDX_DATA_0<<3,
	.unused6 = 0,
	.ds = GDT_IDX_DATA_0<<3,
	.unused7 = 0,
	.fs = GDT_IDX_DATA_0<<3,
	.unused8 = 0,
	.gs = GDT_IDX_DATA_0<<3,
	.unused9 = 0,
	.ldt = 0,
	.unused10 = 0,
	.dtrap = 0,
	.iomap = 0
};

tss_t tss_tasks[22];

void tss_init() {
	vaddr_t cr3 = init_rick();
	tss_tasks[0].ptl = 0;
  	tss_tasks[0].unused0 = 0;
	tss_tasks[0].esp0 = mmu_next_free_kernel_page() + 0x1000;
	tss_tasks[0].ss0 = GDT_IDX_DATA_0 << 3;
	tss_tasks[0].unused1 = 0;
	tss_tasks[0].esp1 = 0;
	tss_tasks[0].ss1 = 0;
	tss_tasks[0].unused2 = 0;
	tss_tasks[0].esp2 = 0;
	tss_tasks[0].ss2 = 0;
	tss_tasks[0].unused3 = 0;
	tss_tasks[0].cr3 = (uint32_t)cr3;
	tss_tasks[0].eip = TASK_CODE_VIRTUAL;
	tss_tasks[0].eflags = 0x202;
	tss_tasks[0].eax = 0;
	tss_tasks[0].ecx = 0;
	tss_tasks[0].edx = 0;
	tss_tasks[0].ebx = 0;
	tss_tasks[0].esp = 0x1D04000;
	tss_tasks[0].ebp = 0x1D04000;
	tss_tasks[0].esi = 0;
	tss_tasks[0].edi = 0;
	tss_tasks[0].es = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[0].unused4 = 0;
	tss_tasks[0].cs = GDT_IDX_CODE_3<<3 | 3;
	tss_tasks[0].unused5 = 0;
	tss_tasks[0].ss = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[0].unused6 = 0;
	tss_tasks[0].ds = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[0].unused7 = 0;
	tss_tasks[0].fs = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[0].unused8 = 0;
	tss_tasks[0].gs = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[0].unused9 = 0;
	tss_tasks[0].ldt = 0;
	tss_tasks[0].unused10 = 0;
	tss_tasks[0].dtrap = 0;
	tss_tasks[0].iomap = 0;

	cr3 = init_morty();
	tss_tasks[1].ptl = 0;
  	tss_tasks[1].unused0 = 0;
	tss_tasks[1].esp0 = mmu_next_free_kernel_page() + 0x1000;
	tss_tasks[1].ss0 = GDT_IDX_DATA_0 << 3;
	tss_tasks[1].unused1 = 0;
	tss_tasks[1].esp1 = 0;
	tss_tasks[1].ss1 = 0;
	tss_tasks[1].unused2 = 0;
	tss_tasks[1].esp2 = 0;
	tss_tasks[1].ss2 = 0;
	tss_tasks[1].unused3 = 0;
	tss_tasks[1].cr3 = (uint32_t)cr3;
	tss_tasks[1].eip = TASK_CODE_VIRTUAL;
	tss_tasks[1].eflags = 0x202;
	tss_tasks[1].eax = 0;
	tss_tasks[1].ecx = 0;
	tss_tasks[1].edx = 0;
	tss_tasks[1].ebx = 0;
	tss_tasks[1].esp = 0x1D04000;
	tss_tasks[1].ebp = 0x1D04000;
	tss_tasks[1].esi = 0;
	tss_tasks[1].edi = 0;
	tss_tasks[1].es = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[1].unused4 = 0;
	tss_tasks[1].cs = GDT_IDX_CODE_3<<3 | 3;
	tss_tasks[1].unused5 = 0;
	tss_tasks[1].ss = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[1].unused6 = 0;
	tss_tasks[1].ds = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[1].unused7 = 0;
	tss_tasks[1].fs = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[1].unused8 = 0;
	tss_tasks[1].gs = GDT_IDX_DATA_3<<3 | 3;
	tss_tasks[1].unused9 = 0;
	tss_tasks[1].ldt = 0;
	tss_tasks[1].unused10 = 0;
	tss_tasks[1].dtrap = 0;
	tss_tasks[1].iomap = 0;

	gdt[GDT_IDX_TSS_INIT].base_15_0 = (uint16_t)((uint32_t)&tss_initial);
	gdt[GDT_IDX_TSS_INIT].base_23_16 = (uint8_t)(((uint32_t)&tss_initial)>>16);
	gdt[GDT_IDX_TSS_INIT].base_31_24 = (uint8_t)(((uint32_t)&tss_initial)>>24);

	gdt[GDT_IDX_TSS_IDLE].base_15_0 = (uint16_t)((uint32_t)&tss_idle);
	gdt[GDT_IDX_TSS_IDLE].base_23_16 = (uint8_t)(((uint32_t)&tss_idle)>>16);
	gdt[GDT_IDX_TSS_IDLE].base_31_24 = (uint8_t)(((uint32_t)&tss_idle)>>24);

	gdt[GDT_IDX_TSS_RICK].base_15_0 = (uint16_t)((uint32_t)&tss_tasks[0]);
	gdt[GDT_IDX_TSS_RICK].base_23_16 = (uint8_t)(((uint32_t)&tss_tasks[0])>>16);
	gdt[GDT_IDX_TSS_RICK].base_31_24 = (uint8_t)(((uint32_t)&tss_tasks[0])>>24);

	gdt[GDT_IDX_TSS_MORTY].base_15_0 = (uint16_t)((uint32_t)&tss_tasks[1]);
	gdt[GDT_IDX_TSS_MORTY].base_23_16 = (uint8_t)(((uint32_t)&tss_tasks[1])>>16);
	gdt[GDT_IDX_TSS_MORTY].base_31_24 = (uint8_t)(((uint32_t)&tss_tasks[1])>>24);

}

void tss_task_init(void) {}

