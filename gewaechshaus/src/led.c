#include "led.h"


const int log_100_prozent_table[100] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5,
		5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14,
		14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32,
		34, 35, 37, 39, 41, 43, 45, 47, 49, 52, 54, 57, 59, 62, 65, 68, 72, 75,
		79, 83, 86, 91, 95, 100 };

const int log_precalc_table[100] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3,
		3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 14, 15, 16, 18, 20, 22,
		24, 26, 29, 32, 35, 38, 42, 46, 50, 55, 60, 66, 72, 79, 87, 95, 105,
		115, 126, 138, 151, 165, 181, 199, 218, 238, 261, 286, 314, 344, 377,
		413, 452, 495, 543, 595, 652, 714, 782, 857, 939, 1029, 1127, 1235,
		1353, 1482, 1624, 1779, 1949, 2136, 2340, 2564, 2809, 3077, 3371, 3693,
		4046, 4433, 4857, 5321, 5830, 6387, 6997, 7666, 8398 };

void TM_LEDS_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Clock for GPIOD */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Alternating functions for pins */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);

	/* Set pins */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void TM_TIMER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/*
	 TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock
	 But, timer has internal PLL, which double this frequency for timer, up to 84MHz
	 Remember: Not each timer is connected to APB1, there are also timers connected
	 on APB2, which works at 84MHz by default, and internal PLL increase
	 this to up to 168MHz

	 Set timer prescaller
	 Timer count frequency is set with

	 timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)

	 In our case, we want a max frequency for timer, so we set prescaller to 0
	 And our timer will have tick frequency

	 timer_tick_frequency = 84000000 / (0 + 1) = 84000000
	 */
	TIM_BaseStruct.TIM_Prescaler = 0;
	/* Count up */
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	/*
	 Set timer period when it have reset
	 First you have to know max value for timer
	 In our case it is 16bit = 65535
	 To get your frequency for PWM, equation is simple

	 PWM_frequency = timer_tick_frequency / (TIM_Period + 1)

	 If you know your PWM frequency you want to have timer period set correct

	 TIM_Period = timer_tick_frequency / PWM_frequency - 1

	 In our case, for 10Khz PWM_frequency, set Period to

	 TIM_Period = 84000000 / 10000 - 1 = 8399

	 If you get TIM_Period larger than max timer value (in our case 65535),
	 you have to choose larger prescaler and slow down timer tick frequency
	 */
	TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	/* Start count on TIM4 */
	TIM_Cmd(TIM4, ENABLE);
}
TIM_OCInitTypeDef TIM_OCStruct;
void TM_PWM_Init(void) {
	//TIM_OCInitTypeDef TIM_OCStruct;

	/* Common settings */

	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OCStruct.TIM_Pulse = 419; /* 25% duty cycle */
	TIM_OC1Init(TIM4, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OCStruct.TIM_Pulse = 419; /* 50% duty cycle */
	TIM_OC2Init(TIM4, &TIM_OCStruct);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OCStruct.TIM_Pulse = 419; /* 75% duty cycle */
	TIM_OC3Init(TIM4, &TIM_OCStruct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OCStruct.TIM_Pulse = 419; /* 100% duty cycle */
	TIM_OC4Init(TIM4, &TIM_OCStruct);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void init_TIM7() {
	SysTick_Config(SystemCoreClock / 100);
	// weitere Initialisierungen durchführen

	// GPIOD Takt einschalten
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;

	// Takt für Timer 7 einschalten
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// Timer7 konfigurieren
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 850;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 300;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBase_InitStructure);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// Timer7 einschalten
	TIM_Cmd(TIM7, ENABLE);

	// Interruptcontroller konfigurieren
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Init(&NVIC_InitStructure);
}

int pwm_c = 0;
char s = 0;
void TIM7_IRQHandler() {
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	//TIM_OCStruct.TIM_Pulse = ((8399 + 1) * log_100_prozent_table[pwm_c]) / 100 - 1;
	TIM_OCStruct.TIM_Pulse = log_precalc_table[pwm_c];
	TIM_OC1Init(TIM4, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);


	if (s == 0)
		pwm_c++;
	else
		pwm_c--;
	if (pwm_c >= 100)
		s = 1;
	else if (pwm_c <= 0) {
		s = 0;
	}

}
