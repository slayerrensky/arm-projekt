/* Includes ------------------------------------------------------------------*/
#include "LED.h"
#include "stm32f4_discovery.h"


#define GREEN  LED4_PIN
#define ORANGE LED3_PIN
#define RED    LED5_PIN
#define BLUE   LED6_PIN
#define ALL_LEDS (GREEN | ORANGE | RED | BLUE) // all leds


LED::LED(void)
{
	STM_EVAL_LEDInit(LED3);

}

LED::~LED(void)
{
	// Do nothing
}

// Turn on the led
void LED::On()
{
	STM_EVAL_LEDOn(LED3);

}

// Turn off the led
void LED::Off()
{
	STM_EVAL_LEDOff(LED3);
}
