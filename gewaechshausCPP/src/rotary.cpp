#include "rotary.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include <stdlib.h>

Rotary *RotaryInstance;

Rotary::Rotary(void){

	EnableSingelton();
	buttonState = false;
	folgezustand = 0;
	position = 0;
	this->Init();

	/* Generate software interrupt: simulate a rising edge applied on EXTI0 line */
	EXTI_GenerateSWInterrupt(EXTI_Line0);

}

void Rotary::Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;

	/* Enable GPIOA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void Rotary::EnableSingelton(void)
{
	RotaryInstance = this;
}

int Rotary::GetPosition(){
	return this->position;
}


bool Rotary::hasButtonPressd()
{
	if (RotaryInstance->button == true )
		return true;
	else
		return false;
}

bool Rotary::resetButtonPressd()
{
	RotaryInstance->button = false;
}

void Rotary::Process(void)
{
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
	now_b = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				TM_INT_RE_Count--;
			} else {
				TM_INT_RE_Count++;
			}
#else
			if (now_b == 1) {
				RotaryInstance->position++;
			} else {
				RotaryInstance->position--;
			}
#endif
		}
	}
}

/*
 *  Rotary Interrupt
 */
extern "C" { void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET )
	{
		//QuadratureEncoderInstance->button=true;
		RotaryInstance->button=true;
	    EXTI_ClearITPendingBit(EXTI_Line9);
	}

//	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
//		/* Process rotary encoder */
//		RotaryInstance->Process();
//
//		/* Clear interrupt bit */
//		EXTI_ClearITPendingBit(EXTI_Line6);
//		//EXTI_ClearITPendingBit(EXTI_Line7);
//	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET )
	{
		int a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
		int b = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
		RotaryInstance->istzustand = a<<1 | b ;
		int myoperator = RotaryInstance->table[RotaryInstance->folgezustand * 4 + RotaryInstance->istzustand];
		switch (myoperator)
		{
			case 1: RotaryInstance->position++; break;
			case 2: RotaryInstance->position--; break;
			default: break;
		}
		RotaryInstance->folgezustand = RotaryInstance->istzustand;
		/* Clear the EXTI */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if( EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		int a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
		int b = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
		RotaryInstance->istzustand = a<<1 | b ;
		int myoperator = RotaryInstance->table[RotaryInstance->folgezustand * 4 + RotaryInstance->istzustand];
		switch (myoperator)
		{
			case 1: RotaryInstance->position++; break;
			case 2: RotaryInstance->position--; break;
			default: break;
		}
		RotaryInstance->folgezustand = RotaryInstance->istzustand;
		/* Clear the EXTI */
		EXTI_ClearITPendingBit(EXTI_Line7);
	}

}}
