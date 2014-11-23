#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/*
 * definitions for the quadrature encoder pins
 */

// Right Motor Channels
#define ENCRA_PIN               GPIO_Pin_6
#define ENCRA_GPIO_PORT         GPIOB
#define ENCRA_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENCRA_SOURCE            GPIO_PinSource6
#define ENCRA_AF                GPIO_AF_TIM8

#define ENCRB_PIN               GPIO_Pin_7
#define ENCRB_GPIO_PORT         GPIOB
#define ENCRB_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ENCRB_SOURCE            GPIO_PinSource7
#define ENCRB_AF                GPIO_AF_TIM8

#define ENCR_TIMER              TIM8
#define ENCR_TIMER_CLK          RCC_APB2Periph_TIM8

#define RIGHT_COUNT()           ENCR_TIMER->CNT

class QuadratureEncoder {
public:
	QuadratureEncoder(void);
	void reset(void);
	int32_t getRotaryPos (void);
	int16_t getRotaryDiff (void);

private:
	void read(void);
	int16_t rotActual;
	int16_t rotOld;
	int16_t rotCount;
	int32_t rotTotal;
};
