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
#include "static_commandos.h"
#include "menu.h"

extern "C" {
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
#include "stm32_ub_systick.h"
}

#define TIME_DELAY              5000000
#define EXPECTING_SENSORS    2
char *OutputString;
char buff[129] = "";
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
	Fassade fassade;
	Menu menu;

	display.SpecialCommand(DISPLAY_ClearDisplay, DISPLAY_SOURCE_REMOUTE);
	display.SetCursorPosition(0,0, DISPLAY_SOURCE_REMOUTE);
	display.SendString("Gewaechshaus", DISPLAY_SOURCE_REMOUTE);
	display.SetCursorPosition(1,0, DISPLAY_SOURCE_REMOUTE);
	display.SendString("Version 0.1", DISPLAY_SOURCE_REMOUTE);

	//FassadeInstance->DisplayMassage("hallo");
	FassadeInstance->InitGewaechshaus();

	char buffer[129] = "";
	float sensoren[TemperaturSensorenInstance->getAnzahlGefunderSensoren()];
	int sleep = 0;
	int sleepDisplay = 0;
	int rotBefore = 0;
	int rotNow = 0;


	while (1) {

		if (terminal.IsCommandoAvalible())
		{
			terminal.ProzessCommando();
		}
		if (xbee.IsCommandoAvalible())
		{
			//if (XbeeInstance->txin > 70)
				xbee.ProzessCommando();
		}

		if (sleep >= 10000)
		{
			// Fensterregelung
			// Hole Temperaturvalues
			FassadeInstance->RegelungFenster();
			// Schaue lookuptable oder Rechner Fensterstand
			// Fensterstand setzen
			//FassadeInstance->Window2Position(20);
		}
		if (sleep % 5000 == 0)
		{
			if (menuShow == false)
			{
				FassadeInstance->UpdateDisplayValues();
			}
		}


#ifdef TYPE_REMOUTE
		if (sleep % 100 == 0)
		{
			rotNow = QuadratureEncoderInstance->getRotaryPos();

			//rotNow=UB_ENCODER_TIM2_ReadPos();
			if (rotBefore != rotNow)
			{
				if (menuShow == false)
				{
					menuShow = true;
					MenuInstance->MenuUP();
				}
				else
				{
					MenuInstance->Cursor(rotBefore-rotNow);
					sleepDisplay = 0;
				}
				rotBefore = rotNow;
			}
			if (QuadratureEncoderInstance->hasButtonPressd())
			{
				if (menuShow == false)
				{
					menuShow = true;
					MenuInstance->MenuUP();
				}
				else
				{
					sleepDisplay = 0;
					MenuInstance->Submit();
				}
				QuadratureEncoderInstance->resetButtonPressd();
			}
		}
		if (sleep % 5000 == 0)
		{
			if (menuShow == false)
			{
				FassadeInstance->UpdateDisplayValues2();
			}
		}
		if (menuShow == true)
		{
			if (sleepDisplay % 5000 == 4999)
			{
				menuShow = false;
				FassadeInstance->UpdateDisplayValues();
			}
			sleepDisplay++;
		}
		if (sleep > 10000)
		{
			sleep = 0;
		}
#endif
		led.On();
		//UB_Systick_Pause_ms(1);
		led.Off();
		//UB_Systick_Pause_ms(1);
		sleep++;

	}
}
