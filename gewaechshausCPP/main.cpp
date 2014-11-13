#include "led.h"
#include "uart.h"
#include "stm32f4xx.h"
#include "string.h"

#define TIME_DELAY              5000000
char buffer[129] = "Hello World, this is a DMA test.\n";

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

	com3.usart3InitDMA(buffer, 20);
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
		if (com3.BufferOut(&bytes)==0)
		{
			com3.uartPutChar(bytes);
		}

    }
}
