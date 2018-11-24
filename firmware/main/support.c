/*-----------------------------------------------------------------------------/
/ Copyright (C) 2018, krakrukra, all rights reserved.
/
/ This is an open source software. Redistribution and use of it in
/ source and binary forms, with or without modification, are permitted
/ provided that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright holder or contributors SHALL NOT BE LIABLE for any damages
/ caused by use of this software.
/-----------------------------------------------------------------------------*/

#include "../cmsis/stm32f0xx.h"
#include "../fatfs/ff.h"

extern volatile unsigned char need_sleepcheck;
extern int main();

static void startup();
static inline void initialize_data(unsigned int* from, unsigned int* data_start, unsigned int* data_end) __attribute__((always_inline));
static inline void initialize_bss(unsigned int* bss_start, unsigned int* bss_end) __attribute__((always_inline));

static void default_handler() __attribute__((interrupt));
static void exti_4_15() __attribute__((interrupt));

//these symbols are declared in the linker script
extern unsigned int __data_start__;
extern unsigned int __data_end__;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
extern unsigned int __text_end__;

void* vectorTable[48] __attribute__(( section(".vectab,\"a\",%progbits@") )) =
  {
    (void*)0x20001FFF,//initial main SP value (8kB RAM size)
    (void*)&startup,//reset vector
    (void*)&default_handler,//NMI
    (void*)&default_handler,//HardFault
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)&default_handler,//SVCall
    (void*)0x00000000,//reserved
    (void*)0x00000000,//reserved
    (void*)&default_handler,//PendSV
    (void*)&default_handler,//SysTick
    (void*)0x00000000,//IRQ0
    (void*)0x00000000,//IRQ1
    (void*)0x00000000,//IRQ2
    (void*)0x00000000,//IRQ3
    (void*)0x00000000,//IRQ4
    (void*)0x00000000,//IRQ5
    (void*)0x00000000,//IRQ6
    (void*)&exti_4_15,//IRQ7
    (void*)0x00000000,//IRQ8
    (void*)0x00000000,//IRQ9
    (void*)0x00000000,//IRQ10
    (void*)0x00000000,//IRQ11
    (void*)0x00000000,//IRQ12
    (void*)0x00000000,//IRQ13
    (void*)0x00000000,//IRQ14
    (void*)0x00000000,//IRQ15
    (void*)0x00000000,//IRQ16
    (void*)0x00000000,//IRQ17
    (void*)0x00000000,//IRQ18
    (void*)0x00000000,//IRQ19
    (void*)0x00000000,//IRQ20
    (void*)0x00000000,//IRQ21
    (void*)0x00000000,//IRQ22
    (void*)0x00000000,//IRQ23
    (void*)0x00000000,//IRQ24
    (void*)0x00000000,//IRQ25
    (void*)0x00000000,//IRQ26
    (void*)0x00000000,//IRQ27
    (void*)0x00000000,//IRQ28
    (void*)0x00000000,//IRQ29
    (void*)0x00000000,//IRQ30
    (void*)0x00000000 //IRQ31
  };

//copies initialized static variable values from ROM to RAM
static inline void initialize_data(unsigned int* from, unsigned int* data_start, unsigned int* data_end)
{
  while(data_start < data_end)
    {
      *data_start = *from;
      data_start++;
      from++;
    }
}

//writes to zero uninitialized static variable values in RAM
static inline void initialize_bss(unsigned int* bss_start, unsigned int* bss_end)
{
  while(bss_start < bss_end)
    {
      *bss_start = 0x00000000U;
      bss_start++;
    }
}

//the very first function that the CPU will run
static void startup()
{
  RCC->CR |= (1<<19)|(1<<16);//enable HSE clock, clock security system
  
  initialize_data(&__text_end__, &__data_start__, &__data_end__);
  initialize_bss(&__bss_start__, &__bss_end__);
  
  while( !(RCC->CR & (1<<17)) );//wait until HSE is ready  
  RCC->CFGR = (1<<0);//use HSE as system clock
  while( !((RCC->CFGR & 0x0F) == 0b0101) );//wait until HSE is used as system clock
  RCC->CR &= ~(1<<0);//disable HSI clock

  main();

  NVIC_SystemReset();//if main() ever returns reset MCU
  return;
}

//default IRQ handler initiates system reset
static void default_handler()
{
  NVIC_SystemReset();
  return;
}

static void exti_4_15() //ADXL345 activity signal ISR
{
  //this ISR is used as a wakeup interrupt and to initiate periodic sleepcheck and battcheck
  if(EXTI->PR & (1<<10)) need_sleepcheck = 1;//if falling edge at 1PPS detected

  EXTI->PR = (1<<14)|(1<<10);//clear EXTI pending bits
  return;
}
