#include "ch455.h"
/*
	Description: Ch455 driver code runs on STM32F429IGT6 packet board.
	Funtion: 		 This code offers a software interface, or access, to the nixie tube and key matrix on the packet board.
	Author: 		 Zhao Yufei
	Edition: 		 v1.0(2022.7.7)
*/
uint8_t Nixietube_Code_Neg[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void Ch455_Init(void){
	uint32_t temp;
	
	RCC->APB1ENR |= 0X1 << 4;
	temp = RCC->APB1ENR;
	TIM6->CR1 |= 0X1 << 3;
	//if(TIM7->CR1 & 0X1 << 3)GPIOB->ODR |= 0X1 << 6;
	TIM6->CNT = 0;
	
	RCC->AHB1ENR |= 0X1 << 2;
	temp = RCC->AHB1ENR;
	GPIOC->OTYPER |= (0X1 << 14) | (0X1 << 15);
	GPIOC->ODR |= (0X1 << 14) | (0X1 << 15);
	GPIOC->MODER &= ~((0X3 << 28) | (0X3 << 30));
	GPIOC->MODER |= (0X1 << 28) | (0X1 << 30);
	TIM6_Delay(9000-1,10000-1);
	
	Ch455_WR(0X48,0X01);//This code must not be modified !
}

void TIM6_Delay(uint16_t arr, uint16_t psc){
	uint32_t temp;
	if(arr == 0)return;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->CR1 |= 0X1;
	while(TIM6->CR1 & 0X1);
	return;
}

void Ch455_WR(uint8_t data1, uint8_t data2){
	uint8_t i = 0;
	TIM6_Delay(900-1,10-1);
	SDA_CLR
	TIM6_Delay(900-1,10-1);
	for(i = 0 ; i < 8 ; i ++){
		SCL_CLR
		TIM6_Delay(900-1,10-1);
		if(data1 & 0X80)SDA_SET
		else SDA_CLR
		TIM6_Delay(900-1,10-1);
		SCL_SET
		TIM6_Delay(900-1,10-1);
		data1 <<= 1;
	}
	//GPIOC->MODER &= ~(0X3 << 30);
	SCL_CLR
	TIM6_Delay(900-1,10-1);

	SDA_SET
	TIM6_Delay(900-1,10-1);
	
	SCL_SET
	TIM6_Delay(900-1,10-1);
	//GPIOC->MODER |= 0X1 << 30;
	for(i = 0 ; i < 8 ; i ++){
		SCL_CLR
		TIM6_Delay(900-1,10-1);
		if(data2 & 0X80)SDA_SET
		else SDA_CLR
		TIM6_Delay(900-1,10-1);
		SCL_SET
		TIM6_Delay(900-1,10-1);
		data2 <<= 1;
	}
	//GPIOC->MODER &= ~(0X3 << 30);
	SCL_CLR
	TIM6_Delay(900-1,10-1);
	
	SDA_SET
	TIM6_Delay(900-1,10-1);
	
	SCL_SET
	TIM6_Delay(900-1,10-1);
	//GPIOC->MODER |= 0X3 << 30;
	
	SCL_CLR
	TIM6_Delay(900-1,10-1);
	SDA_CLR
	TIM6_Delay(900-1,10-1);
	SCL_SET
	TIM6_Delay(900-1,10-1);
	SDA_SET
	TIM6_Delay(900-1,10-1);
}

uint8_t Ch455_RD(void){
	uint8_t i = 0;
	uint8_t data1 = 0X4F;
	uint8_t result = 0;
	TIM6_Delay(900-1,10-1);
	SDA_CLR
	TIM6_Delay(900-1,10-1);
	for(i = 0 ; i < 8 ; i ++){
		SCL_CLR
		TIM6_Delay(900-1,10-1);
		if(data1 & 0X80)SDA_SET
		else SDA_CLR
		TIM6_Delay(900-1,10-1);
		SCL_SET
		TIM6_Delay(900-1,10-1);
		data1 <<= 1;
	}
	//GPIOC->MODER &= ~(0X3 << 30);
	SCL_CLR
	TIM6_Delay(900-1,10-1);

	SDA_SET
	TIM6_Delay(900-1,10-1);
	
	SCL_SET
	TIM6_Delay(900-1,10-1);
	
	GPIOC->MODER &= ~(0X3 << 30);
	for(i = 0 ; i < 8 ; i ++){
		result <<= 1;
		SCL_CLR
		TIM6_Delay(900-1,10-1);
		if(GPIOC->IDR & 0X1 << 15)result += 1;
		else;
		TIM6_Delay(900-1,10-1);
		SCL_SET
		TIM6_Delay(900-1,10-1);
	}
	//GPIOC->MODER &= ~(0X3 << 30);
	SCL_CLR
	TIM6_Delay(900-1,10-1);
	
	SDA_SET
	GPIOC->MODER |= 0X1 << 30;
	TIM6_Delay(900-1,10-1);
	
	SCL_SET
	TIM6_Delay(900-1,10-1);
	//GPIOC->MODER |= 0X3 << 30;
	
	SCL_CLR
	TIM6_Delay(900-1,10-1);
	SDA_CLR
	TIM6_Delay(900-1,10-1);
	SCL_SET
	TIM6_Delay(900-1,10-1);
	SDA_SET
	TIM6_Delay(900-1,10-1);
	return result;
}


void Set_Nixie_tube(uint8_t num, uint8_t val){
	if(num > 4)return;
	if(val > 15)return;
	Ch455_WR(0X68 + num * 2,Nixietube_Code_Neg[val]);
	return;
}

uint8_t Read_Keys(void){
	uint8_t temp = Ch455_RD();
	if(temp & 0X1 << 6){
		temp = (temp & 0X3) + ((temp & 0X38) >> 1) + 1;
		return temp;
	}
	else return 0;
}