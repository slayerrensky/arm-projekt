#include "led.h"
#include "uart.h"
#include "rotary.h"
#include "stm32f4xx.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#define TIME_DELAY              1000000
char buffer[129] = "Hello World, this is a DMA test.\n\r";
char *OutputString;
// Delay Function
static void Delay(volatile int tick)
{
	while(tick--);
}

LED LPC1768_LED;


// Main function entry
int main(void)
{
	/* Initialize system */
	SystemInit();
	Usart com3(128);
	Rotary rotary;
	com3.EnableSingelton();

	com3.SendViaDma(buffer, strlen(buffer));
	com3.SendViaDma(buffer, strlen(buffer));
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

		//com3.SendViaDma(buffer, strlen(buffer));
		//Delay
		Delay(TIME_DELAY);
		OutputString = com3.ReadBuffer();
		if (OutputString != NULL)
		{
			com3.SendViaDma(OutputString, strlen(OutputString));
			free(OutputString);
		}

		sprintf(buffer, "Position: %d \n\r", rotary.GetPosition());
		com3.SendViaDma(buffer, strlen(buffer));

    }
}
