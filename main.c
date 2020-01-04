#include "stm32l476xx.h"
#include "SysTimer.h"
#include "FinalLabLib.h"
#include "lcd.h"
#include "adc.h"
#include <math.h>

void System_Clock_Init(void);
void Set_Reflect_Out(void);
void Set_Reflect_In(void);
void Delay(void);
char check_reflec(int);
void check_reflectance(void);
void Check_dist(void);
volatile uint32_t result;
int mode = 0;
volatile uint8_t combo[6];
volatile uint8_t comboD[2];
int checkDist(void);
void Delay2(int);

int main(){
	unsigned int idr_test, idr_test1;
	int r = 0;
	unsigned int i,k;
	int delay = 0;
	int SystemCoreClock = 100000;
	int line = 0, maze = 0, art = 1;
	int accel = 0;
	int flag = 0;
	int curr_Dist;

    MotorInit();
	System_Clock_Init();
	LCD_Initialization();
	LCD_Clear();
	ADC_Init();
	if(SysTick_Config(SystemCoreClock)) {
		while(1);
	}

	  while(art){
		mode = 4;
		mode = 0;
		Delay2(70000);
		mode = 2;
		Delay2(20000);
			
		}
	  while(maze){
			if(checkDist() > 12 && checkDist() < 17){ 
				mode = 0;
			}
			else if(checkDist() <= 12){
				mode =3;
				if(checkDist() <= 9){
					mode = 1;
					Delay2(19000);
					mode = 3;
					Delay2(30000);
			  }
		  }
			
			// far from wall
			else if(checkDist() >= 17){
				mode = 2;
				if(checkDist() >= 25){
					mode = 0;
					Delay2(60000);
					mode = 2;
					Delay2(25000);
				}
			}

		}
			

	while(line){
    	Set_Reflect_Out();
		Delay();
		Set_Reflect_In();
		check_reflectance();
		mode = 0;
		if(combo[1] == 'B'){
			
			mode = 2;
			SystemCoreClock = 75000;
		} 
		if(combo[2] == 'B'){
			mode = 3;
			SystemCoreClock = 75000;
		}
		if (combo[2] == 'W' && combo[1] == 'W' && combo[3] == 'B'){
			mode = 0; 
			SystemCoreClock = 47000;
		}
		if (combo[2] == 'B' && combo[1] == 'B' && combo[3] == 'B'){
			 mode = 0;
			SystemCoreClock = 47000;
		}
		if (combo[1] == 'B' && combo[2] == 'B' && combo[3] == 'B') mode = 0;
		accel = accel + 1;
		if (accel > 20000) accel = 20000; 

		if(SysTick_Config(SystemCoreClock)) {
		while(1);
	}
  }


void Set_Reflect_Out(void){
	GPIOA->MODER &= ~(0xCF3);
	GPIOA->MODER |= (0x451);
	GPIOA->ODR |= (0x2D);
	return;
}

void Set_Reflect_In(){
	GPIOA->ODR &= ~(0x2D);
	GPIOA->MODER &= ~(0xCF3);
	return;
}
void check_reflectance(void){
	unsigned int flag[4] = {0,0,0,0};
	int count[4] = {0,0,0,0};
	int distance, tens, ones;
	char tens_c, ones_c;
	int ascii = 48;
	int i = 0;
	count[0] = 0;
	combo[0] = 'W';
	combo[1] = 'W';
	combo[2] = 'W';
	combo[3] = 'W';
	combo[4] = '0';
	combo[5] = '0';
	for(i = 0; i < 5000; i++){
		if((GPIOA->IDR &= (0x1)) == 0 && (flag[0] == 0)){
				count[0] = i;
			  flag[0] = 1;
		}
		if((GPIOA->IDR &= (0x1 << 2)) == 0 && flag[1] == 0){
				count[1] = i;
				flag[1] = 1;
		}
		if((GPIOA->IDR &= (0x1 << 3)) == 0 && flag[2] == 0){
				count[2] = i;
			  flag[2] = 1;
		}			
		if((GPIOA->IDR &= (0x1 << 5)) == 0 && flag[3] == 0){
				count[3] = i;
			  flag[3] = 1;
		}
	}
	if (count[0] > 300)
		combo[0] = 'B';
	if (count[1] > 350)
		combo[1] = 'B';
	if (count[2]  > 350)
		combo[2] = 'B';
	if (count[3] > 350)
		combo[3] = 'B';
	LCD_DisplayString(&combo[0]);
	return ;
}

int checkDist(void){
	int distance, tens, ones;
	char tens_c, ones_c;
	int ascii = 48;
	ADC1->CR |= ADC_CR_ADSTART;			
	while ( (ADC123_COMMON->CSR | ADC_CSR_EOC_MST) == 0);
	result = ADC1->DR;
	distance = 1 / (0.00001 * pow(((double) result),1.1392));
	if (distance > 90) distance = 90;
	tens = (int) distance / 10;
	ones = (int) distance - (10 * tens);
	tens_c = (char) (tens + ascii);
	ones_c = (char) (ones + ascii);
	comboD[0] = tens_c;
	comboD[1] = ones_c;
	LCD_DisplayString(&comboD[0]);
	return distance;
}

void Delay(void){
	int wait_time;
	wait_time = 20 * (80000000 / 1000000);
	while(wait_time != 0) {
		wait_time--;
	}   
}

void Delay2(int delay){
	int wait_time;
	wait_time = delay * (80000000 / 1000000);
	while(wait_time != 0) {
		wait_time--;
	}   
}

void SysTick_Handler(void){
  direction(mode);
}