#include "led.h"
#include "uart.h"
#include "stm32f4xx.h"

#define TIME_DELAY              5000000

// Delay Function
static void Delay(volatile int tick)
{
	while(tick--);
}

LED LPC1768_LED;
Usart com1(128);

// Main function entry
int main(void)
{
	/* Initialize system */
	//SystemInit();

	while(1)
    {
		// Turn all four color led on
		LPC1768_LED.On();
		com1.uartSendString("LED ON\r\n");
		// Delay
		Delay(TIME_DELAY);

		// Turn all four color led off
		LPC1768_LED.Off();
		com1.uartSendString("LED OFF\r\n");
		// Delay
		Delay(TIME_DELAY);


    }
}
