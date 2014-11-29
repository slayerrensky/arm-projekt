#include "stm32f4xx.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "led.h"
#include "uart.h"
#include "quadrature_encoder.h"
#include "adc.h"
#include "temp_sensors.h"

extern "C" {
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
#include "stm32_ub_systick.h"
}

#define TIME_DELAY              5000000
#define EXPECTING_SENSORS    2
char buffer[129] = "Hello World, this is a DMA test.\n\r";
char *OutputString;

LED LPC1768_LED;

// Main function entry
int main(void) {
	/* Initialize system */
	SystemInit();

	Usart com3(128);
	QuadratureEncoder rotary;
	AnalogDigitalConverter adc;
	TemperaturSensoren tempSensors;

	com3.EnableSingelton();

	sprintf(buffer, "Es wurden %d Temperatursensoren gefunden.\n",
			tempSensors.getAnzahlGefunderSensoren());
	com3.uartSendString(buffer);

	while (1) {
		UB_Systick_Pause_ms(500);
		tempSensors.startTempMeasurementAllSensors();
		OutputString = com3.ReadBuffer();
		if (OutputString != NULL) {
			com3.SendViaDma(OutputString, strlen(OutputString));
			free(OutputString);
		}
		//if (rotary.isRotDiff()) {
		sprintf(buffer, "RotDiff: %4d ADC Value: %5d (%1.3fV) Temp0: %3.2f C\n",
				rotary.getRotaryDiff(), adc.getConvertedValue(),
				adc.getConvertedValueAsVoltage(),
				tempSensors.getTempWertFromSensor(0));
		com3.SendViaDma(buffer, strlen(buffer));
		//}
	}
}
