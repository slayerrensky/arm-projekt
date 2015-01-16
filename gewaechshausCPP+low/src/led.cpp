/* Includes ------------------------------------------------------------------*/
#include "LED.h"



#define GREEN  LED4_PIN
#define ORANGE LED3_PIN
#define RED    LED5_PIN
#define BLUE   LED6_PIN
#define ALL_LEDS (GREEN | ORANGE | RED | BLUE) // all leds


Led::Led(void)
{
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED5);
}

Led::~Led(void)
{
	// Do nothing
}

// Turn on the led
void Led::On(Led_TypeDef i)
{
	STM_EVAL_LEDOn(i);

}

// Turn off the led
void Led::Off(Led_TypeDef i)
{
	STM_EVAL_LEDOff(i);
}
