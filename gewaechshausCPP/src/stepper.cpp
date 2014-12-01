/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
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

#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 0

Stepper *StepperInstance;
const uint8_t steps[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
int currentStep = 0;
int stepperEnd = 0;
int direction = DIRECTION_LEFT;

Stepper::Stepper(void){

	EnableSingelton();
	this->Init();
}

void Stepper::Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOD clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PE pins as Output floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

/*
 * t in ms
 */
void Stepper::Left(int stepps,int time)
{
	stepperEnd = stepps;
	direction = DIRECTION_LEFT;
	InitTim2(40000, 500);

}

/*
 * t in ms
 */
void Stepper::Right(int stepps,int time)
{
	stepperEnd = stepps;
	direction = DIRECTION_RIGHT;
	InitTim2(40000, 500);

}

void Stepper::RunStep()
{
	uint8_t j;
	for (j = 0; j < 4; j++) {
		if (steps[currentStep%4][j] == 0) {
			GPIO_ResetBits(GPIOD, 1 << (j+8) );
		} else {
			GPIO_SetBits(GPIOD, 1 << (j+8) );
		}
	}
	currentStep++;
}

void Stepper::EnableSingelton(void)
{
	StepperInstance = this;
}

void Stepper::InitTim2(int prescaler, int period)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = prescaler;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);

	TIM_Cmd(TIM2, ENABLE);
}

/*
 *  Stepper Interrupt
 */
extern "C" void TIM2_IRQHandler()
 {
     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
     {
         if (currentStep < stepperEnd)
         {
        	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        	 StepperInstance->RunStep();
         }
         else
         {
        	 TIM_Cmd(TIM2, DISABLE);
         }
     }
 }

