#include "temp_sensors.h"

TemperaturSensoren *TemperaturSensorenInstance;

TemperaturSensoren::TemperaturSensoren(void) {
	TemperaturSensorenInstance = this;
	gefundeTempSensoren = 0;

	//TM_OneWire_Init();

	/* Initialize delay */
	TM_DELAY_Init();

	/*Initialize OneWire on pin PD0 */
	TM_OneWire_Init(&OneWire1, GPIOD, GPIO_Pin_0, RCC_AHB1Periph_GPIOD);

	gefundeTempSensoren = TM_OneWire_First(&OneWire1);

	uint8_t count = 0;
//	for (uint8_t i; i < gefundeTempSensoren; i++) {
//		TM_OneWire_GetFullROM(&sensorAdressen[i - 1][0]);
//		TM_OneWire_Next();
//	}
	while (gefundeTempSensoren) {
		count++;
		//Get full ROM value, 8 bytes, give location of first byte where to save
		TM_OneWire_GetFullROM (&OneWire1, &sensorAdressen[count - 1][0]);
		//Get next device
		gefundeTempSensoren = TM_OneWire_Next(&OneWire1);
	}
	gefundeTempSensoren = count;
	if (gefundeTempSensoren <= 0)
		return;

	// 12 Bit Aufloesung einstellen
	for (uint8_t i = 0; i < gefundeTempSensoren; i++) {
		TM_DS18B20_SetResolution(&OneWire1,&sensorAdressen[i][0],
				TM_DS18B20_Resolution_12bits);
	}
}

void TemperaturSensoren::startTempMeasurementAllSensors(void) {
	TM_DS18B20_StartAll(&OneWire1);
	//Wait until all are done
	while (!TM_DS18B20_AllDone(&OneWire1))
		;
	//Read temperature from each device separatelly
	for (uint8_t i = 0; i < gefundeTempSensoren; i++) {
		//Read temperature from ROM address and store it to temps variable
		TM_DS18B20_Read(&OneWire1, &sensorAdressen[i][0], &sensorTemperaturen[i]);
	}
}

uint8_t TemperaturSensoren::getAnzahlGefunderSensoren(void) {
	return gefundeTempSensoren;
}

void TemperaturSensoren::getAlleTempWerte(float *tempArray) {
	for (uint8_t i=0; i < gefundeTempSensoren; i++) {
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
