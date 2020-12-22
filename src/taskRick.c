#include "stddef.h"
#include "syscall.h"
#include "i386.h"

void meeseks1_func(void);

void A(uint32_t llamado);
void B(uint32_t llamado);
void C(uint32_t llamado);

uint32_t MAX_LLAMADOS = 10;
uint32_t llamado = 0;

void task(void) {
  while (1) {
    syscall_meeseeks((uint32_t)&meeseks1_func, 0, 0);
    llamado++;
  }
}

void meeseks1_func(void) {
  A(llamado);
  while(1){
    __asm volatile("nop");
    
  }
}


void A(uint32_t llamado){
  if(llamado >= MAX_LLAMADOS){
    uint8_t *p = 0;
    p[0] = 42;
  }else{
    B(llamado+1);
  }
}

void B(uint32_t llamado){
  if(llamado >= MAX_LLAMADOS){
    uint8_t *p = 0;
    p[0] = 42;
  }else{
    C(llamado+1);
  }
}

void C(uint32_t llamado){
  if(llamado >= MAX_LLAMADOS){
    uint8_t *p = 0;
    p[0] = 42;
  }else{
    A(llamado+1);
  }
}