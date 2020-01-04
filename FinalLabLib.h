#ifndef __FINALLabLib_H
#define __FINALLabLib_H

#include "stm32l476xx.h"

void assign(unsigned int set);
void direction(int mode);
void assign2(unsigned int set);
void MotorInit(void);
void System_Clock_Init(void);
void SysTick_Handler(void);

#endif 