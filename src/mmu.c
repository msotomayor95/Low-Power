/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

void mmu_init(void) {
	NEXT_FREE_KERNEL_PAGE = FREE_KERNEL_SPACE;
	
}

paddr_t mmu_next_free_kernel_page(void) {
  paddr_t actual = NEXT_FREE_KERNEL_PAGE;
  NEXT_FREE_KERNEL_PAGE += 0x1000;
  return actual;
}

// paddr_t mmu_next_free_task_page(void) {
//   paddr_t actual = NEXT_FREE_TASK_PAGE;
//   NEXT_FREE_TASK_PAGE += 0x1000;
//   return actual;
// }

paddr_t mmu_init_kernel_dir(void) {
	pd_entry *pd = (pd_entry *)KERNEL_PAGE_DIR;
  	pt_entry *pt = (pt_entry *)KERNEL_PAGE_TABLE_0;

  	for (int i = 0; i < 1024; ++i) {
  		pd[i] = (pd_entry){0};
  		pt[i] = (pt_entry){0};
  	}

  	mmu_kernel_identity_mapping(pd, pt);

  	return (paddr_t)pd;
}

void mmu_kernel_identity_mapping(pd_entry *pd, pt_entry *pt) {

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
}

 void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint8_t rw, uint8_t supervisor) {
 	pd_entry *pd = (pd_entry *)((cr3>>12)<<12);
 	uint32_t pd_entry = virt>>22;
 	if (pd[pd_entry].present != 1) {
 		pd[pd_entry].read_write = rw;
 		pd[pd_entry].user_supervisor = supervisor;
 		pd[pd_entry].present = 1;
 		paddr_t new_page = mmu_next_free_kernel_page();		
 		pd[pd_entry].page_table_base = new_page>>12;
 	}
 	pt_entry *pt = (pt_entry *) ((pd[pd_entry].page_table_base)<<12);
 	uint32_t pt_entry = ((virt<<10)>>22);
 	pt[pt_entry].present = 1;
 	pt[pt_entry].read_write = rw;
 	pt[pt_entry].user_supervisor = supervisor;
 	pt[pt_entry].physical_address_base = phy>>12;

 }

 paddr_t mmu_unmap_page (uint32_t cr3, vaddr_t virt) {
 	pd_entry *pd = (pd_entry *) ((cr3>>12)<<12);
 	uint32_t pd_entry = virt>>22;
 	pt_entry *pt = (pt_entry *) ((pd[pd_entry].page_table_base)<<12);
 	uint32_t pt_entry = ((virt<<10)>>22);
 	pt[pt_entry].present = 0;
 	return 0;
 }

 paddr_t mmu_init_task_dir (paddr_t phy_start, paddr_t code_start, size_t pages, vaddr_t v_start, uint8_t rw, uint8_t user_supervisor) {
 	
 	// como map_page crea una tabla de paginas nueva, uso esta funcion para ahorrar codigo
 	// en vez de pasarle un cr3, le pasamos la direccion al nuevo directorio de pagina
 	// no es necesario ningun shift previo ya que todas las direcciones se mueven de a 0x1000
 	// y el area libre para paginas empieza en 100000
 	uint32_t new_dir = mmu_next_free_kernel_page();
 	pd_entry *pd = (pd_entry *) new_dir;
 	pt_entry *pt = (pt_entry *) mmu_next_free_kernel_page(); 
 	for (int i = 0; i < 1024; ++i) {
  		pd[i] = (pd_entry){0};
  		pt[i] = (pt_entry){0};
  	}

  	mmu_kernel_identity_mapping(pd_entry *pd, pt_entry *pt);
  	vaddr_t v_temp = v_start;
 	for (int i = 0; i < 4; ++i) {
 		mmu_map_page(new_dir, v_temp, phy_start, rw, user_supervisor);
 		phy_start = phy_start + 0x1000;
 		v_temp = v_temp + 0x1000;
 	}

 	uint8_t *byte_pointer_source = (uint8_t *) code_start;
  	uint8_t *byte_pointer_destiny = (uint8_t *) v_start;
  	for (int i = 0; i < 0x1000 * pages; ++i)
  	{
  		byte_pointer_destiny[i] = byte_pointer_source[i];
  	}
 	return 0;
 }

