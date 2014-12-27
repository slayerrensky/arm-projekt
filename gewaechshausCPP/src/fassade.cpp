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


void Fassade::Window2Position(void){
	int current = AnalogDigitalConverterInstance->getConvertedValue(ADC_CHANNEL_VOLTAGE);
	int position = current * (int)(2.44 + 0.5);
#ifdef DEBUG
	sprintf(buffer, "\r\nADV_Value: %d -> Go2Step: %d \r\n",current, position);
	TerminalInstance->SendMessage(buffer);
#endif
	StepperInstance->Go2Step(position);

}

