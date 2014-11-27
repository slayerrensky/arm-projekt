#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"


class AnalogDigitalConverter {
public:
	AnalogDigitalConverter(void);
	uint16_t getConvertedValue(void);
private:
	uint16_t convertedValue;
};
#endif
