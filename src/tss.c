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

	for (int i = GDT_IDX_TSS_RICK; i < GDT_COUNT; ++i) {
		gdt[i].base_15_0 = (uint16_t)((uint32_t)&tss_tasks[i]);
		gdt[i].base_23_16 = (uint8_t)(((uint32_t)&tss_tasks[i])>>16);
		gdt[i].base_31_24 = (uint8_t)(((uint32_t)&tss_tasks[i])>>24);
	}

}

void tss_task_init(uint32_t index, vaddr_t code_start, uint8_t x, uint8_t y) { 
	uint32_t map_offset = 80*y*8*1024 + x*8*1024;
	paddr_t map_dir = BASE_MAP + map_offset;				// 8 * 1024 = 2 paginas. 80*y*8*1024 + x*8*1024
	uint32_t stack0_index = index;
	index = index + BASE_TSS;
	tss_tasks[index] = tss_tasks[index % 2];				// igualo la tss de la tarea que estoy creando al de su creador (rick = 0 o morty = 1)

	// solo hay unos campos que difieren del meeseek con su creador
	// selecciono una pagina de 20 que tenia previamente inicializadas
	tss_tasks[index].esp0 = stack0[stack0_index] + 0x1000;

	// dir virtual mapeada a la primera de las 2 paginas de la tarea
	tss_tasks[index].eip = code_start; 
	// dir virtual mapeada a la direccion de la segunda pagina de la tarea
	tss_tasks[index].esp = code_start + 0x1000*0x2 ; // el final de la segunda pagina?
	tss_tasks[index].ebp = code_start + 0x1000*0x2;
}

