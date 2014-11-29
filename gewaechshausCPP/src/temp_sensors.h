#ifndef TEMP_SENSORS_H_
#define TEMP_SENSORS_H_

//#include "stm32f4xx_tim.h"
#include "stdint.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
extern "C" {
//#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
//#include "stm32_ub_systick.h"
}

#define MAX_EXPECTED_SENSORS 4

class TemperaturSensoren {
public:
	TemperaturSensoren(void);
	void startTempMeasurementAllSensors(void);
	uint8_t getAnzahlGefunderSensoren(void);
	float getTempWertFromSensor(uint8_t index);
	void getAlleTempWerte(float *tempArray);

private:
	uint8_t gefundeTempSensoren;
	uint8_t sensorAdressen[MAX_EXPECTED_SENSORS][8];
	float sensorTemperaturen[MAX_EXPECTED_SENSORS];
};
#endif
