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
#include "uart.h"
#include "led.h"



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

	usart3Init();
	uartSendString("Hello Word!\r\n");

	while (1) {

	}
}
