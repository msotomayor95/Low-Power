/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

void mmu_init(void) {}

paddr_t mmu_next_free_kernel_page(void) {
  return 0;
}

paddr_t mmu_init_kernel_dir(void) {
	pd_entry *pd = (pd_entry *)KERNEL_PAGE_DIR;
  	pt_entry *pt = (pt_entry *)KERNEL_PAGE_TABLE_0;

  	for (int i = 0; i < 1024; ++i) {
  		pd[i] = (pd_entry){0};
  		pt[i] = (pt_entry){0};
  	}

  	pd[0].present = MMU_FLAG_PRESENT;
  	pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
  	pd[0].read_write = MMU_FLAG_READWRITE;
  	pd[0].page_table_base = ((uint32_t) pt)>>12;

  	for (int i = 0; i < 1024; ++i) {
  		pt[i].present = MMU_FLAG_PRESENT;
  		pt[i].user_supervisor = MMU_FLAG_SUPERVISOR;
  		pt[i].read_write = MMU_FLAG_READWRITE;
  		pt[i].physical_address_base = i;
  	}

  	return (paddr_t)pd;
}

// void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {}
// paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {}
// paddr_t mmu_init_task_dir(paddr_t phy_start, paddr_t code_start, size_t
// pages) {}

