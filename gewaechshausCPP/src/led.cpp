/* Includes ------------------------------------------------------------------*/
#include "LED.h"
#include "stm32f4_discovery.h"


#define GREEN  LED4_PIN
#define ORANGE LED3_PIN
#define RED    LED5_PIN
#define BLUE   LED6_PIN
#define ALL_LEDS (GREEN | ORANGE | RED | BLUE) // all leds


Led::Led(void)
{
	STM_EVAL_LEDInit(LED3);

}

Led::~Led(void)
{
	// Do nothing
}

// Turn on the led
void Led::On()
{
	STM_EVAL_LEDOn(LED3);

}

// Turn off the led
void Led::Off()
{
	STM_EVAL_LEDOff(LED3);
}
