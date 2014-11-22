#include "quadrature_encoder.h"

QuadratureEncoder::QuadratureEncoder(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Connect the pins to their Alternate Functions
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

	// set them up as encoder inputs
	// set both inputs to rising polarity to let it use both edges
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICInitTypeDef TIM_ICInit_struct;
	TIM_ICStructInit(&TIM_ICInit_struct);
	TIM_ICInit_struct.TIM_Channel = TIM_Channel_1 || TIM_Channel_2 || TIM_Channel_3 || TIM_Channel_4;
	TIM_ICInit_struct.TIM_ICFilter = 6;
	TIM_ICInit(TIM8, &TIM_ICInit_struct);

	TIM_SetAutoreload(TIM8, 0xffff);

	// turn on the timer/counters
	TIM_Cmd(TIM8, ENABLE);
	reset();
}

void QuadratureEncoder::reset(void) {
	__disable_irq();
	this->rotActual = 0;
	this->rotTotal = 0;
	//TIM_SetCounter(TIM8, 0);
	//read();
	__enable_irq();
}

void QuadratureEncoder::read(void) {
	rotOld = rotActual;
	rotActual = TIM_GetCounter(TIM8);
	rotCount = rotActual - rotOld;
	rotTotal += rotCount;
}

int32_t QuadratureEncoder::getRotaryPos(void) {
	read();
	return rotTotal;
}

int16_t QuadratureEncoder::getRotaryDiff(void) {
	read();
	return rotCount;
}
