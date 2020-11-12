/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
#include "defines.h"
#include "isr.h"
#include "screen.h"

idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/

#define IDT_ENTRY(numero) \
    idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF); \
    idt[numero].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3); \
    idt[numero].attr = (uint16_t) 0x8E00; \
    idt[numero].offset_31_16 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);


void idt_init() {
  // Excepciones
  IDT_ENTRY(0);
  IDT_ENTRY(1);
  IDT_ENTRY(2);
  IDT_ENTRY(3);
  IDT_ENTRY(4);
  IDT_ENTRY(5);
  IDT_ENTRY(6);
  IDT_ENTRY(7);
  IDT_ENTRY(8);
  IDT_ENTRY(9);
  IDT_ENTRY(10);
  IDT_ENTRY(11);
  IDT_ENTRY(12);
  IDT_ENTRY(13);
  IDT_ENTRY(14);
  IDT_ENTRY(15);
  IDT_ENTRY(16);
  IDT_ENTRY(17);
  IDT_ENTRY(18);
  IDT_ENTRY(19);
  IDT_ENTRY(20);
  IDT_ENTRY(21);
  IDT_ENTRY(22);
  IDT_ENTRY(23);
  IDT_ENTRY(24);
  IDT_ENTRY(25);
  IDT_ENTRY(26);
  IDT_ENTRY(27);
  IDT_ENTRY(28);
  IDT_ENTRY(29);
  IDT_ENTRY(30);
  IDT_ENTRY(31);

  // Clock
  idt[32].offset_15_0 = (uint16_t) ((uint32_t)(&_isrClock) & (uint32_t) 0xFFFF);        
  idt[32].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[32].attr = (uint16_t) 0x8E00;                                                          
  idt[32].offset_31_16 = (uint16_t) ((uint32_t)(&_isrClock) >> 16 & (uint32_t) 0xFFFF);

  idt[33].offset_15_0 = (uint16_t) ((uint32_t)(&_isrKey) & (uint32_t) 0xFFFF);        
  idt[33].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[33].attr = (uint16_t) 0x8E00;                                                          
  idt[33].offset_31_16 = (uint16_t) ((uint32_t)(&_isrKey) >> 16 & (uint32_t) 0xFFFF);

  idt[88].offset_15_0 = (uint16_t) ((uint32_t)(&_isr88) & (uint32_t) 0xFFFF);        
  idt[88].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[88].attr = (uint16_t) 0xEE00;                                                          
  idt[88].offset_31_16 = (uint16_t) ((uint32_t)(&_isr88) >> 16 & (uint32_t) 0xFFFF);

  idt[89].offset_15_0 = (uint16_t) ((uint32_t)(&_isr89) & (uint32_t) 0xFFFF);        
  idt[89].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[89].attr = (uint16_t) 0xEE00;                                                          
  idt[89].offset_31_16 = (uint16_t) ((uint32_t)(&_isr89) >> 16 & (uint32_t) 0xFFFF);

  idt[100].offset_15_0 = (uint16_t) ((uint32_t)(&_isr100) & (uint32_t) 0xFFFF);        
  idt[100].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[100].attr = (uint16_t) 0xEE00;                                                          
  idt[100].offset_31_16 = (uint16_t) ((uint32_t)(&_isr100) >> 16 & (uint32_t) 0xFFFF);

  idt[123].offset_15_0 = (uint16_t) ((uint32_t)(&_isr123) & (uint32_t) 0xFFFF);        
  idt[123].segsel = (uint16_t) (GDT_IDX_CODE_0 << 3);                                         
  idt[123].attr = (uint16_t) 0xEE00;                                                          
  idt[123].offset_31_16 = (uint16_t) ((uint32_t)(&_isr123) >> 16 & (uint32_t) 0xFFFF);

}

