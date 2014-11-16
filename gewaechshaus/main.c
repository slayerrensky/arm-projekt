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
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "uart.h"
#include "led.h"
#include "string.h"

char hello[] = "Hello World!\r\n";

int main(void) {
	/* Initialize system */
	SystemInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* Init leds */
	//TM_LEDS_Init();
	/* Init timer */
	//TM_TIMER_Init();
	/* Init PWM */
	//TM_PWM_Init();
	//init_TIM7();

	usart3Init();
	SendViaDma(hello, 10);//strlen(hello));
	//SendViaDma(hello, strlen(hello));
	while (1) {
		while (DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET);
		DMA_DeInit(DMA1_Stream3);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		SendViaDma(hello, strlen(hello));

	}
}
