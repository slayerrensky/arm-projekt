#include "temp_sensors.h"

TemperaturSensoren *TemperaturSensorenInstance;

TemperaturSensoren::TemperaturSensoren(void) {
	TemperaturSensorenInstance = this;
	gefundeTempSensoren = 0;

	TM_OneWire_Init();

	gefundeTempSensoren = TM_OneWire_First();

	uint8_t count = 0;
//	for (uint8_t i; i < gefundeTempSensoren; i++) {
//		TM_OneWire_GetFullROM(&sensorAdressen[i - 1][0]);
//		TM_OneWire_Next();
//	}
	while (gefundeTempSensoren) {
		count++;
		//Get full ROM value, 8 bytes, give location of first byte where to save
		TM_OneWire_GetFullROM (&sensorAdressen[count - 1][0]);
		//Get next device
		gefundeTempSensoren = TM_OneWire_Next();
	}
	gefundeTempSensoren = count;
	if (gefundeTempSensoren <= 0)
		return;

	// 12 Bit Aufloesung einstellen
	for (uint8_t i = 0; i < gefundeTempSensoren; i++) {
		TM_DS18B20_SetResolution(&sensorAdressen[i][0],
				TM_DS18B20_Resolution_12bits);
	}
}

void TemperaturSensoren::startTempMeasurementAllSensors(void) {
	TM_DS18B20_StartAll();
	//Wait until all are done
	while (!TM_DS18B20_AllDone())
		;
	//Read temperature from each device separatelly
	for (uint8_t i = 0; i < gefundeTempSensoren; i++) {
		//Read temperature from ROM address and store it to temps variable
		TM_DS18B20_Read(&sensorAdressen[i][0], &sensorTemperaturen[i]);
	}
}

uint8_t TemperaturSensoren::getAnzahlGefunderSensoren(void) {
	return gefundeTempSensoren;
}

void TemperaturSensoren::getAlleTempWerte(float *tempArray) {
	for (uint8_t i; i < gefundeTempSensoren; i++) {
		tempArray[i] = sensorTemperaturen[i];
	}
}

float TemperaturSensoren::getTempWertFromSensor(uint8_t index) {
	if ((index >= MAX_EXPECTED_SENSORS) || (index < 0)) {
		//fehlerfall
		return 0.0;
	}
	return sensorTemperaturen[index];
}
