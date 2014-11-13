#include "led.h"
#include "uart.h"
#include "stm32f4xx.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#define TIME_DELAY              5000000
char buffer[129] = "Hello World, this is a DMA test.\n";
char *OutputString;
// Delay Function
static void Delay(volatile int tick)
{
	while(tick--);
}

LED LPC1768_LED;
Usart com3(128);

// Main function entry
int main(void)
{
	/* Initialize system */
	//SystemInit();
	//Usart com3(128);
	char bytes;


	while(1)
    {
		// Turn all four color led on
		//LPC1768_LED.On();
		//com3.uartSendString("LED ON\r\n");
		// Delay
		//Delay(TIME_DELAY);

		// Turn all four color led off
		//LPC1768_LED.Off();
		//com3.uartSendString("LED OFF\r\n");
		// Delay
		//Delay(TIME_DELAY);

		OutputString = com3.ReadBuffer();
		com3.SendViaDma(OutputString, strlen(OutputString));
		free(OutputString);

    }
}
