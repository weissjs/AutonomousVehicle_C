#include "FinalLabLib.h"

unsigned int FullStep[4] = {0x9, 0xA, 0x6, 0x5};
unsigned int HalfStep[8] = {0x9, 0x8, 0xA, 0x2, 0x6, 0x4, 0x5, 0x1};

int mode1, mode2, step1 = 0, step2 = 0;
int step = 0;

void assign(unsigned int set){
	GPIOE->ODR &= ~(0xF << 12);
	GPIOE->ODR |= (set << 12);
	return;
}

void assign2(unsigned int set){
	GPIOB->ODR &= ~(0xCC);
	GPIOB->ODR |= ((set & 0xC) << 4);
	GPIOB->ODR |= ((set & 0x3) << 2);
	return;
}

void direction(int mode){
	if (mode == 0){                // forward
		step1 = ((step) % 8);
		step2 = 8 - (step + 7)%8;
	}
	if (mode == 1){								//backward
		step1 = 8 - ((step + 7) % 8);
		step2 = ((step) % 8);
	}
	if (mode == 2){								// right
		step1 = ((step) % 8);
		step2 = ((step) % 8);
	}
	if (mode == 3){								// left
		step1 = 8 - ((step + 7) % 8);
		step2 = 8 - ((step + 7) % 8);
	}
	if (mode == 4){
		step1 = 0;
		step2 = 0;
	}
	assign(HalfStep[step1]);
	assign2(HalfStep[step2]);
	if (mode ==4) step = step;                 //no movement
	else
	  step+=2;
	return;
}
void MotorInit(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOE->MODER &= ~(0xFF000000);
	GPIOE->MODER |= 0x55 << 24;
	GPIOB->MODER &= ~(0xF0F << 4);
	GPIOB->MODER |= (0x505 << 4);
}

void System_Clock_Init(void){
	
	RCC->CR |= RCC_CR_MSION; 
	
	// Select MSI as the clock source of System Clock
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 	
	
	// MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1). 
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= RCC_CR_MSIRANGE_7;  // Select MSI 8 MHz	

	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 		
}
