/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */
#include "stepper.h"

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include <stdlib.h>



Stepper *StepperInstance;
//                               A0,A1,B0,B1
const uint8_t steps[2][8][4] = {{{1, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0},{0, 1, 0, 1}, {0, 0, 0, 1},{1, 0, 0, 1},{1, 0, 0, 0}},
								{{1, 0, 1, 0}, {1, 0, 0, 0}, {1, 0, 0, 1}, {0, 0, 0, 1}, {0, 1, 0, 1},{0, 1, 0, 0}, {0, 1, 1, 0},{0, 0, 1, 0}}} ;

Stepper::Stepper(void){

	EnableSingelton();
	this->Init();
}

void Stepper::Init(void)
{
	StepperInstance->StepperStatus = STEPPER_USABLE;
	StepperInstance->currentStep = 0;
	StepperInstance->stepperEnd = 0;
	StepperInstance->direction = DIRECTION_LEFT;
	StepperInstance->position = 0;

	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOD clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PE pins as Output floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, Bit_RESET);
}

/*
 * t in ms
 */
void Stepper::Left(int stepps,int time)
{
	if (StepperInstance->StepperStatus == STEPPER_USABLE)
	{
		StepperInstance->StepperStatus = STEPPER_BUSSY;
		StepperInstance->stepperEnd = stepps;
		StepperInstance->direction = DIRECTION_LEFT;

		// FRQ = 84MHz / (Prescaler+1) / (Periode+1)
		StepperInstance->InitTim2(84-1, 1000 - 1); //10HZ
	}
}

/*
 * t in ms
 */
void Stepper::Right(int stepps,int time)
{
	if (StepperInstance->StepperStatus == STEPPER_USABLE)
	{
		StepperInstance->StepperStatus = STEPPER_BUSSY;
		StepperInstance->stepperEnd = stepps;
		StepperInstance->direction = DIRECTION_RIGHT;
		StepperInstance->InitTim2(84, 1000 - 1); //10HZ
	}

}

void Stepper::Go2Step(int g){
	int n;
	if (g < StepperInstance->position)
	{
		n = StepperInstance->position - g;
		StepperInstance->Left(n,10);
	} else if ( g > StepperInstance->position)
	{
		n = g - StepperInstance->position;
		StepperInstance->Right(n,10);
	}
}

void Stepper::Leerlauf()
{
	StepperInstance->currentStep = 0;
	StepperInstance->StepperStatus = STEPPER_USABLE;
	GPIO_ResetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 );
}

void Stepper::RunStep()
{
	uint8_t j;
	for (j = 0; j < 4; j++) {
		if (steps[StepperInstance->direction][StepperInstance->currentStep%8][j] == 0) {
			GPIO_ResetBits(GPIOE, 1 << (j+7) );
		} else {
			GPIO_SetBits(GPIOE, 1 << (j+7) );
		}
	}
	if (StepperInstance->direction == DIRECTION_RIGHT)
	{
		StepperInstance->position++;
	}
	else
	{
		StepperInstance->position--;
	}
	StepperInstance->currentStep++;
}

void Stepper::EnableSingelton(void)
{
	StepperInstance = this;
}

void Stepper::InitTim2(int prescaler, int period)
{
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = prescaler;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

/*
 *  Stepper Interrupt
 */
extern "C" void TIM2_IRQHandler()
 {
     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
     {
         if (StepperInstance->currentStep < StepperInstance->stepperEnd)
         {
        	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        	 StepperInstance->RunStep();
         }
         else
         {
        	 TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        	 TIM_Cmd(TIM2, DISABLE);
        	 StepperInstance->Leerlauf();
         }
     }
 }

