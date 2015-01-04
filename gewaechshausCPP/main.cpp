#include "stm32f4xx.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "led.h"
#include "quadrature_encoder.h"
#include "adc.h"
#include "temp_sensors.h"
#include "stepper.h"
#include "ampermeter.h"
#include "terminal.h"
#include "display.h"
#include "fassade.h"
#include "xbee.h"
#include "stm32f4xx_usart.h"
#include "defines.h"

extern "C" {
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
#include "stm32_ub_systick.h"
}

#define TIME_DELAY              5000000
#define EXPECTING_SENSORS    2
//char buffer[129] = "Hello World, this is a DMA test.\n\r";
char *OutputString;

Led led;

// Main function entry
int main(void) {
	/* Initialize system */
	SystemInit();

	Terminal terminal(128);
	terminal.EnableSingelton();
	QuadratureEncoder rotary;
	AnalogDigitalConverter adc;
	TemperaturSensoren tempSensors;
	Stepper step;
	Ampermeter current;
	Display display(20);
	Xbee xbee(256, XBEE_TYPE_CORE);
//	int TimerCount;
	Fassade fassade(TYPE_CORE);

	display.SpecialCommand(DISPLAY_ClearDisplay);
	display.SetCursorPosition(0);
	display.SendString("Gewaechshaus");
	display.SetCursorPosition(20);
	display.SendString("Version 0.1");

	FassadeInstance->DisplayMassage("hallo");
	FassadeInstance->InitGewaechshaus();
	FassadeInstance->RegelungFenster();

	while (1) {
		//UB_Systick_Pause_ms(500);
		//tempSensors.startTempMeasurementAllSensors();
		//OutputString = com3.ReadBuffer();


		if (terminal.IsCommandoAvalible())
		{
			terminal.ProzessCommando();
		}
		if (xbee.IsCommandoAvalible())
		{
			xbee.ProzessCommando();
		}
//		char wert = USART_ReceiveData(USART2);
		// Fensterregelung

		// Hole Temperaturvalues
		// Schaue lookuptable oder Rechner Fensterstand
		// Fensterstand setzen
		//FassadeInstance->Window2Position(20);

		led.On();
		UB_Systick_Pause_ms(100);
		led.Off();

	}
}
