#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#define ADC_CHANNEL_CURRENT 0
#define ADC_CHANNEL_VOLTAGE 1
#define ADC_MAX_VALUE 4096

class AnalogDigitalConverter {
public:
	AnalogDigitalConverter(void);
	uint16_t getConvertedValue(int channel);
	float getConvertedValueAsVoltage(int channel);
private:
	uint16_t convertedValue;
};

extern AnalogDigitalConverter *AnalogDigitalConverterInstance;
#endif
