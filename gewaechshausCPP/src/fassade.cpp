/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */

#define DEBUG
#include "fassade.h"
#include "stepper.h"
#include "terminal.h"
#include "adc.h"
#include "temp_sensors.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>

char buffer[129] = "";
Fassade *FassadeInstance;

Fassade::Fassade(){

}

void Fassade::InitGewaechshaus(void){
		sprintf(buffer, "Es wurden %d Temperatursensoren gefunden.\r\n",
				TemperaturSensorenInstance->getAnzahlGefunderSensoren());
		 TerminalInstance->SendMessage(buffer);
}

/*
 * Steuerung der Fenster Aufgrund eines Prozent Werts
 */
void Fassade::Window2Position(int inProzent){
	//int voltage = AnalogDigitalConverterInstance->getConvertedValue(ADC_CHANNEL_VOLTAGE);
	//int position = voltage * (int)(2.44 + 0.5); //Scritte pro millivolt
	int position = (int) ((ADC_MAX_VALUE / 100.0 * inProzent)+0.5);
#ifdef DEBUG
	//sprintf(buffer, "\r\nADV_Value: %d -> Go2Step: %d \r\n",voltage, position);
	sprintf(buffer, "\r\nNew Stepper Position: %d \r\n", position);
	TerminalInstance->SendMessage(buffer);
#endif
	StepperInstance->Go2Step(position);

}

