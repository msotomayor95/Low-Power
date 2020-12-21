#include "stddef.h"
#include "syscall.h"
#include "i386.h"


uint32_t WAIT_TIME = 500000;

void meeseeks_problematico(void);
void task(void) {
  while(1){
      syscall_meeseeks((uint32_t)&meeseeks_problematico, 0, 0);
      for (uint32_t i = 0; i < WAIT_TIME; ++i)
      {
        __asm volatile("nop");
      }
      syscall_meeseeks((uint32_t)&meeseeks_problematico, 0, 1);
      for (uint32_t i = 0; i < WAIT_TIME; ++i)
      {
        __asm volatile("nop");
      }
      syscall_meeseeks((uint32_t)&meeseeks_problematico, 0, 2);
      for (uint32_t i = 0; i < WAIT_TIME; ++i)
      {
        __asm volatile("nop");
      }


  }
}

void meeseeks_problematico(void) {
  for (uint32_t i = 0; i < WAIT_TIME; ++i)
  {
    __asm volatile("nop");
  }
  uint8_t* kernel_free_area_start = (uint8_t*) 0x100000;
  int i = 0;
  while (1) {
    // breakpoint();
    kernel_free_area_start[i] = 0x0;
    i++;
  }
}
