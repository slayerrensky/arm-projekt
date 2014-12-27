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

extern "C" {
#include "tm_stm32f4_onewire.h"
#include "tm_stm32f4_ds18b20.h"
#include "stm32_ub_systick.h"
}

#define TIME_DELAY              5000000
#define EXPECTING_SENSORS    2
//char buffer[129] = "Hello World, this is a DMA test.\n\r";
char *OutputString;

LED LPC1768_LED;

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
//	int TimerCount;



	while (1) {
		//UB_Systick_Pause_ms(500);
		//tempSensors.startTempMeasurementAllSensors();
		//OutputString = com3.ReadBuffer();
		if (terminal.IsCommandoAvalible())
		{
			terminal.ProzessCommando();
		}



		//if (rotary.isRotDiff()) {
//		TimerCount = rotary.getRotaryDiff();
//		sprintf(buffer, "RotDiff: %4d, ADC Value: %1.3fV, Current: %2.4fA, Temp0: %3.2f C\r\n",
//				TimerCount,
//				adc.getConvertedValueAsVoltage(1),
//				current.getCurrent(),
//				tempSensors.getTempWertFromSensor(0));
//		terminal.SendMessage(buffer);
//		if (TimerCount > 0 )
//			step.Left(TimerCount,20);
//		else if (TimerCount < 0 )
//		{
//			step.Right(TimerCount,20);
//
//		}

		//}
	}
}
