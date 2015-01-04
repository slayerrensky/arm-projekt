/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */

#include "fassade.h"
#include "stepper.h"
#include "terminal.h"
#include "adc.h"
#include "temp_sensors.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>
#include "xbee.h"
#include "defines.h"


char buffer[129] = "";
Fassade *FassadeInstance;

Fassade::Fassade(int type){
	FassadeInstance = this;
	this->type = type;
}

void Fassade::InitGewaechshaus(void){
		sprintf(buffer, "Es wurden %d Temperatursensoren gefunden.\r\n",
				TemperaturSensorenInstance->getAnzahlGefunderSensoren());
		 TerminalInstance->SendMessage(buffer);
}

void Fassade::RegelungFenster(void)
{
	int i;
	float sensoren[TemperaturSensorenInstance->getAnzahlGefunderSensoren()];
	TemperaturSensorenInstance->getAlleTempWerte(sensoren);
	for (i=0; i<TemperaturSensorenInstance->getAnzahlGefunderSensoren(); i++)
	{
		sprintf(buffer, "Sensor %d value %f3.3 .\r\n",i, sensoren[i]);
		TerminalInstance->SendMessage(buffer);
	}
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

void Fassade::DisplayMassage(char *massage){

#ifdef TYPE_CORE
	XbeeInstance->SendTransmission(XBEE_PROTOKOLL_VERSION, XBEE_TYPE_REMOUTE, (char)0x10,(char) 0x00,  massage ,(char)strlen(massage));
#endif
}


