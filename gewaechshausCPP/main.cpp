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
	Fassade fassade(TYPE_CORE);

	display.SpecialCommand(DISPLAY_ClearDisplay);
	display.SetCursorPosition(0,0);
	display.SendString("Gewaechshaus");
	display.SetCursorPosition(1,0);
	display.SendString("Version 0.1");

	FassadeInstance->DisplayMassage("hallo");
	//FassadeInstance->InitGewaechshaus();
	//FassadeInstance->RegelungFenster();

	sprintf(buff, "Es wurden %d Temperatursensoren gefunden.\r\n",
			tempSensors.getAnzahlGefunderSensoren());
	terminal.SendMessage(buff);
	int i;
	tempSensors.startTempMeasurementAllSensors();
	float sensoren[4];
	tempSensors.getAlleTempWerte(sensoren);
	for (i=0; i<tempSensors.getAnzahlGefunderSensoren(); i++)
	{
		sprintf(buff, "Sensor %d value %.3f .\r\n",i, sensoren[i]);
		terminal.SendMessage(buff);
	}

	char buffer[129] = "";
	float sensoren[TemperaturSensorenInstance->getAnzahlGefunderSensoren()];

	while (1) {
		UB_Systick_Pause_ms(500);
		tempSensors.startTempMeasurementAllSensors();

		tempSensors.startTempMeasurementAllSensors();
			float sensoren[4];
			tempSensors.getAlleTempWerte(sensoren);
			for (i=0; i<4; i++)
			{
				sprintf(buff, "Sensor %d value %.3f .\r\n",i, sensoren[i]);
				terminal.SendMessage(buff);
			}

		//OutputString = com3.ReadBuffer();
		TemperaturSensorenInstance->startTempMeasurementAllSensors();
		TemperaturSensorenInstance->getAlleTempWerte(sensoren);
		DisplayInstance->SpecialCommand(DISPLAY_ClearDisplay);
		for (int i=0; i<TemperaturSensorenInstance->getAnzahlGefunderSensoren(); i++)
		{
			sprintf(buffer, "Sensor %d: %3.2f",i, sensoren[i]);
			DisplayInstance->SetCursorPosition(i,0);
			DisplayInstance->SendString(buffer);
		}

		//if (terminal.IsCommandoAvalible())
		//{
		//	terminal.ProzessCommando();
		//}
		//if (xbee.IsCommandoAvalible())
		//{
		//	xbee.ProzessCommando();
		//}
//		char wert = USART_ReceiveData(USART2);
		// Fensterregelung

		// Hole Temperaturvalues
		// Schaue lookuptable oder Rechner Fensterstand
		// Fensterstand setzen
		//FassadeInstance->Window2Position(20);

		led.On();
		UB_Systick_Pause_ms(1);
		led.Off();
		UB_Systick_Pause_ms(1);

	}
}
