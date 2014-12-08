#include "ampermeter.h"
//#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include "adc.h"
//#include "stm32f4xx_dma.h"

Ampermeter *AmpermeterInstance;

Ampermeter::Ampermeter(void) {

}
float Ampermeter::getCurrent(void) {
	return (AnalogDigitalConverterInstance->getConvertedValueAsVoltage(0) -1.65)/0.09;
}
