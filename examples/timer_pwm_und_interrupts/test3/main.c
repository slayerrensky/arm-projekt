/**
 *    PWM example for STM32F4 Discovery
 *    It should work on STM32F429 Discovery too and all other STM32F4xx devices
 *
 *    @author     Tilen Majerle
 *    @email        tilen@majerle.eu
 *    @website    http://stm32f4-discovery.com
 *    @ide        Keil uVision 5
 */
//#include "defines.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

const int log_100_prozent_table[100] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5,
		5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14,
		14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32,
		34, 35, 37, 39, 41, 43, 45, 47, 49, 52, 54, 57, 59, 62, 65, 68, 72, 75,
		79, 83, 86, 91, 95, 100 };
const int log_100_prozent_table_von_NULL[100] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5,
		5, 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21,
		23, 24, 26, 28, 30, 32, 35, 37, 40, 43, 46, 49, 53, 57, 61, 65, 70, 75,
		81, 86, 93, 100 };

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
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	/* Set pins */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
			| GPIO_Pin_15;
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

	/*
	 To get proper duty cycle, you have simple equation

	 pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

	 where DutyCycle is in percent, between 0 and 100%

	 25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	 50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	 75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	 100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

	 Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
	 */
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

	// Konfiguriere PD15
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Takt für Timer 7 einschalten
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// Timer7 konfigurieren
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 850;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 320;
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

	TIM_OC3Init(TIM4, &TIM_OCStruct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	if (s == 0)
		pwm_c++;
	else
		pwm_c--;
	if (pwm_c >= 101)
		s = 1;
	else if (pwm_c <= 0) {
		s = 0;
		pwm_c = 0;
	}
}

int main(void) {
	/* Initialize system */
	SystemInit();
	/* Init leds */
	TM_LEDS_Init();
	/* Init timer */
	TM_TIMER_Init();
	/* Init PWM */
	TM_PWM_Init();
	init_TIM7();

	while (1) {

	}
}

//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include <stddef.h>
//#include <stdlib.h>
//#include "stm32f4xx_tim.h"
//#include "stm32f4xx_exti.h"
//#include "misc.h"
//#include "stm32f4xx_rcc.h"
//#include <math.h>
//
//#define LED_GREEN      12
//#define LED_ORANGE     13
//#define LED_RED        14
//#define LED_BLUE       15
//int pwmtable[256] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
//		2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6,
//		7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15, 15,
//		16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33,
//		35, 36, 38, 40, 41, 43, 45, 47, 49, 52, 54, 56, 59, 61, 64, 67, 70, 73,
//		76, 79, 83, 87, 91, 95, 99, 103, 108, 112, 117, 123, 128, 134, 140, 146,
//		152, 159, 166, 173, 181, 189, 197, 206, 215, 225, 235, 245, 256, 267,
//		279, 292, 304, 318, 332, 347, 362, 378, 395, 412, 431, 450, 470, 490,
//		512, 535, 558, 583, 609, 636, 664, 693, 724, 756, 790, 825, 861, 899,
//		939, 981, 1024, 1069, 1117, 1166, 1218, 1272, 1328, 1387, 1448, 1512,
//		1579, 1649, 1722, 1798, 1878, 1961, 2048, 2139, 2233, 2332, 2435, 2543,
//		2656, 2773, 2896, 3025, 3158, 3298, 3444, 3597, 3756, 3922, 4096, 4277,
//		4467, 4664, 4871, 5087, 5312, 5547, 5793, 6049, 6317, 6596, 6889, 7194,
//		7512, 7845, 8192, 8555, 8933, 9329, 9742, 10173, 10624, 11094, 11585,
//		12098, 12634, 13193, 13777, 14387, 15024, 15689, 16384, 17109, 17867,
//		18658, 19484, 20346, 21247, 22188, 23170, 24196, 25267, 26386, 27554,
//		28774, 30048, 31378, 32768, 34218, 35733, 37315, 38967, 40693, 42494,
//		44376, 46340, 48392, 50534, 52772, 55108, 57548, 60096, 62757, 65535 };
//
//void init_led(int pin) {
//	uint32_t mode = GPIO_Mode_OUT << (pin * 2);
//	uint32_t speed = GPIO_Speed_100MHz << (pin * 2);
//	uint32_t type = GPIO_OType_PP << pin;
//	uint32_t pullup = GPIO_PuPd_NOPULL << (pin * 2);
//	//
//	//  Initialise the peripheral clock.
//	//
//	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
//	//
//	//  Initilaise the GPIO port.
//	//
//	GPIOD->MODER |= mode;
//	GPIOD->OSPEEDR |= speed;
//	GPIOD->OTYPER |= type;
//	GPIOD->PUPDR |= pullup;
//}
//
//void initApplication() {
//	SysTick_Config(SystemCoreClock / 100);
//	// weitere Initialisierungen durchführen
//
//	// GPIOD Takt einschalten
//	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
//
//	// Konfiguriere PD15
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//	// Takt für Timer 7 einschalten
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
//
//	// Timer7 konfigurieren
//	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
//	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBase_InitStructure.TIM_Period = 1;
//	TIM_TimeBase_InitStructure.TIM_Prescaler = 1;
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBase_InitStructure);
//	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
//
//	// Timer7 einschalten
//	TIM_Cmd(TIM7, ENABLE);
//
//	// Interruptcontroller konfigurieren
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//	NVIC_Init(&NVIC_InitStructure);
//}
//
//int counter2 = 0;
//int c1 = 1;
//int pwm_b = 0;
//int main() {
//	init_led(LED_BLUE);
//	init_led(LED_RED);
//	init_led(LED_GREEN);
//	init_led(LED_ORANGE);
//	//GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
//	//GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
//	//initApplication();
//	while (1) {
//		if (counter2++ >= 65535)
//			counter2 = 0;
//		if (counter2 == 0)
//			c1++;
//		if (c1 >= 256)
//			c1 = 0;
//		pwm_b = pwmtable[c1];
//		if (counter2 >= pwm_b)
//			GPIOD->BSRRH = (1 << LED_BLUE);
//		else
//			GPIOD->BSRRL = (1 << LED_BLUE);
//	}
//	//
//	//  Toggle the selected LED indefinitely.
//	//
////	int index;
////	while (1) {
////		GPIOD->BSRRL = (1 << LED_BLUE);
////
////		for (index = 0; index < 90000; index++)
////			;
////		GPIOD->BSRRH = (1 << LED_BLUE);
////		GPIOD->BSRRL = (1 << LED_RED);
////
////		for (index = 0; index < 90000; index++)
////			;
////		GPIOD->BSRRH = (1 << LED_RED);
////		GPIOD->BSRRL = (1 << LED_ORANGE);
////
////		for (index = 0; index < 90000; index++)
////			;
////		GPIOD->BSRRH = (1 << LED_ORANGE);
////		GPIOD->BSRRL = (1 << LED_GREEN);
////
////		for (index = 0; index < 90000; index++)
////					;
////		GPIOD->BSRRH = (1 << LED_GREEN);
////	}
//
//	volatile int pwm_green = 0;
//	volatile int pwm_blue = 0;
//	volatile int pwm_orange = 0;
//	volatile int pwm_red = 0;
//	volatile int counter = 0;
//	volatile char sign = 0;
//
//	while (1) {
//
//		GPIOD->BSRRL = (1 << LED_BLUE);
//		GPIOD->BSRRL = (1 << LED_RED);
//		GPIOD->BSRRL = (1 << LED_GREEN);
//		GPIOD->BSRRL = (1 << LED_ORANGE);
//
//		int i = 0;
//		for (i = 0; i < 1020; i++) {
//			if (i >= pwm_blue)
//				GPIOD->BSRRH = (1 << LED_BLUE);
//			if (i >= pwm_green)
//				GPIOD->BSRRH = (1 << LED_GREEN);
//			if (i >= pwm_red)
//				GPIOD->BSRRH = (1 << LED_RED);
//			if (i >= pwm_orange)
//				GPIOD->BSRRH = (1 << LED_ORANGE);
//		}
//
//		if (counter++ >= 3) {
//			counter = 0;
//			if (sign == 0) {
//				pwm_blue += 10;
//				pwm_red += 10;
//				pwm_green += 10;
//				pwm_orange += 10;
//			} else {
//				pwm_blue -= 10;
//				pwm_red -= 10;
//				pwm_green -= 10;
//				pwm_orange -= 10;
//			}
//
//			if (pwm_blue == 1020)
//				sign = 1;
//			if (pwm_blue == 0)
//				sign = 0;
//		}
//	}
//}
//
//void SysTick_Handler(void) {
//	//leer
//}
//
//void TIM7_IRQHandler() {
//	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
//	if (counter2++ >= 65535)
//		counter2 = 0;
//	if (counter2 >= 65535)
//		c1 += c1;
//	if (c1 >= 256)
//		c1 = 0;
//	pwm_b = pwmtable[c1];
//	if (counter2 >= pwm_b)
//		GPIOD->BSRRH = (1 << LED_BLUE);
//	else
//		GPIOD->BSRRL = (1 << LED_BLUE);
//	//GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
//	/*GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
//	 GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
//	 GPIO_ToggleBits(GPIOD,GPIO_Pin_15);*/
//}
//
