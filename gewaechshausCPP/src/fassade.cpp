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
#include "display.h"
#include "defines.h"
#include "ampermeter.h"
#include "adc.h"


char bufferT[256] = "";
float solltemp;

Fassade *FassadeInstance;

Fassade::Fassade(){
	Fassade(TYPE_CORE);
}

Fassade::Fassade(int type){
	FassadeInstance = this;
	this->type = type;
}

void Fassade::InitGewaechshaus(void){
		sprintf(bufferT, "Es wurden %d Temperatursensoren gefunden.\r\n",
				TemperaturSensorenInstance->getAnzahlGefunderSensoren());
		 TerminalInstance->SendMessage(bufferT);
		 SetSolltemp(24.0);
}

void Fassade::RegelungFenster(void)
{

	int anzahl = TemperaturSensorenInstance->getAnzahlGefunderSensoren();
	TemperaturSensorenInstance->startTempMeasurementAllSensors();
	float sensoren[anzahl];
	TemperaturSensorenInstance->getAlleTempWerte(sensoren);
#ifdef DEBUG
	int i;
	for (i=0; i<anzahl; i++)
	{
		sprintf(bufferD, "Sensor %d: %3.2f",i, sensoren[i]);
		DisplayInstance->SetCursorPosition(i,0);
		DisplayInstance->SendString(bufferD);

	}
#endif
	float in = sensoren[TEMP_SENSOR_IN];
	float out = sensoren[TEMP_SENSOR_OUT];
#ifdef DEBUG
	sprintf(bufferT, "Outdor: %.3f C.\r\n",out);
			TerminalInstance->SendMessage(bufferT);
	sprintf(bufferT, "Indor : %.3f C.\r\n",in);
				TerminalInstance->SendMessage(bufferT);
#endif
	float difference;
	int dif = 0;

	if (GetSolltemp() > in && in < out && GetSolltemp() > out) // drin zu kalt, draußen kalt aber wärmer als drin, fenster auf
	{
		difference = GetSolltemp() - in;
		if (difference > 0 )
		{
			dif = 0;
			if (difference > 10)
				dif = 10;
			else
				dif = (int)difference;
		}
		else
			dif = 0;
	}
	else if ( GetSolltemp() > in && GetSolltemp() <= out) // drin zu kalt, draußen wärmer, fenster auf
	{
			difference = GetSolltemp() - in;
			if (difference > 0 )
			{
				dif = 0;
				if (difference > 10)
					dif = 10;
				else
					dif = (int)difference;
			}
			else
				dif = 0;
		}
	else if (GetSolltemp() > in && out <= in) // drin zu kalt, draußen kälter, fenster zu
	{
			dif = 0;
	}
	else if ( GetSolltemp() < in && out < in) // drin zu warm, draußen kälter, fenster auf
	{
		difference = in - GetSolltemp();
		if (difference > 0 )
		{
			dif = 0;
			if (difference > 10)
				dif = 10;
			else
				dif = (int)difference;
		}
		else
			dif = 0;
	}
	else if ( GetSolltemp() < in && out > in) // drin zu warm, draußen wärmer, fenster auf
	{
		difference = in - GetSolltemp();
		if (difference > 0 )
		{
			dif = 0;
			if (difference > 10)
				dif = 10;
			else
				dif = (int)difference;
		}
		else
			dif = 0;
	}

#ifdef DEBUG
	sprintf(bufferT, "Difference %d \r\n",dif);
		TerminalInstance->SendMessage(bufferT);
#endif
	FassadeInstance->Window2Position(TempReglungLUT[0][dif]);
	//FassadeInstance->Lueftung(TempReglungLUT[1][dif]);

}



/*
 * Steuerung der Fenster Aufgrund eines Prozent Werts
 */
void Fassade::Window2Position(int inProzent){
	//int voltage = AnalogDigitalConverterInstance->getConvertedValue(ADC_CHANNEL_VOLTAGE);
	//int position = voltage * (int)(2.44 + 0.5); //Scritte pro millivolt
	int position = (int) ((STEPPER_MAX_POSITION / 100.0 * inProzent)+0.5);
#ifdef DEBUG
	//sprintf(bufferT, "\r\nADV_Value: %d -> Go2Step: %d \r\n",voltage, position);
	sprintf(bufferT, "\r\nNew Stepper Position: %d \r\n", position);
	TerminalInstance->SendMessage(bufferT);
#endif
	StepperInstance->Go2Step(position);

}

void Fassade::UpdateDisplayValues(){

	DisplayInstance->SpecialCommand(DISPLAY_ClearDisplay,DISPLAY_SOURCE_LOCAL);


	sprintf(bufferD, "Outdoor : %3.2f",XbeeInstance->values.outdor);
	DisplayInstance->SetCursorPosition(1,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);
	sprintf(bufferD, "Indoor  : %3.2f",XbeeInstance->values.indor);
	DisplayInstance->SetCursorPosition(0,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);
	sprintf(bufferD, "Sollwert: %3.2f",XbeeInstance->values.sollwert);
	DisplayInstance->SetCursorPosition(2,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);
	sprintf(bufferD, "Fenster : %d", XbeeInstance->values.windowP);
	DisplayInstance->SetCursorPosition(3,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);
}

void Fassade::UpdateDisplayValues2(){

	DisplayInstance->SpecialCommand(DISPLAY_ClearDisplay,DISPLAY_SOURCE_LOCAL);

	sprintf(bufferD, "Current : %1.3f A",XbeeInstance->values.current);
	DisplayInstance->SetCursorPosition(0,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);
	sprintf(bufferD, "Voltage : %1.3f V",XbeeInstance->values.voltage);
	DisplayInstance->SetCursorPosition(1,0, DISPLAY_SOURCE_LOCAL);
	DisplayInstance->SendString(bufferD, DISPLAY_SOURCE_LOCAL);

}

void Fassade::SendValuesToRemoteDisplay()
{
	int anzahl = TemperaturSensorenInstance->getAnzahlGefunderSensoren();
	TemperaturSensorenInstance->startTempMeasurementAllSensors();
	float sensoren[anzahl];
	TemperaturSensorenInstance->getAlleTempWerte(sensoren);

	XbeeInstance->values.current =  AmpermeterInstance->getCurrent();
	XbeeInstance->values.voltage =  AnalogDigitalConverterInstance->getConvertedValueAsVoltage(ADC_CHANNEL_VOLTAGE);
	XbeeInstance->values.windowP =  StepperInstance->GetPositionInProzent();
	XbeeInstance->values.sollwert = solltemp;
	XbeeInstance->values.indor = sensoren[TEMP_SENSOR_IN];
	XbeeInstance->values.outdor = sensoren[TEMP_SENSOR_OUT];
	XbeeInstance->TransmittPValues();
}


void Fassade::TerminalDisplayTemp()
{
	int anzahl = TemperaturSensorenInstance->getAnzahlGefunderSensoren();
	TemperaturSensorenInstance->startTempMeasurementAllSensors();
	float sensoren[anzahl];
	TemperaturSensorenInstance->getAlleTempWerte(sensoren);
	float in = sensoren[TEMP_SENSOR_IN];
	float out = sensoren[TEMP_SENSOR_OUT];

	sprintf(bufferT, "Outdor: %.3f C.\r\n",out);
			TerminalInstance->SendMessage(bufferT);
	sprintf(bufferT, "Indor : %.3f C.\r\n",in);
			TerminalInstance->SendMessage(bufferT);
}

void Fassade::SendMassageToDisplay(char *massage){

#ifdef TYPE_CORE
	XbeeInstance->SendTransmission(XBEE_PROTOKOLL_VERSION, XBEE_TYPE_REMOUTE, (char)0x10,(char) 0x00,  massage ,(char)strlen(massage));
#endif
#ifdef TYPE_BOTH
	DisplayInstance->SendString(massage, DISPLAY_SOURCE_LOCAL);
#endif
}

void Fassade::SetSolltemp(float t){
	solltemp = t;
}

float Fassade::GetSolltemp(void){
	return solltemp;
}


