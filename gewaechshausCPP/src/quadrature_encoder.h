#ifndef QUADRATURE_ENCODER_H_
#define QUADRATURE_ENCODER_H_

#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

class QuadratureEncoder {
public:
	QuadratureEncoder(void);
	void reset(void);
	int32_t getRotaryPos (void);
	int16_t getRotaryDiff (void);
	bool isRotDiff(void);

private:
	void read(void);
	int16_t rotActual;
	int16_t rotOld;
	int16_t rotCount;
	int32_t rotTotal;
};
#endif
