///* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
// * Owner: René Galow and Dennis Rump
// *
// */
//#include "quadrature_encoder.h"
//
//QuadratureEncoder *QuadratureEncoderInstance;
//
//QuadratureEncoder::QuadratureEncoder(void) {
//	QuadratureEncoderInstance=this;
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
//
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//GPIO_StructInit(&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	// Connect the pins to their Alternate Functions
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
//
//	// set them up as encoder inputs
//	// set both inputs to rising polarity to let it use both edges
//	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI1, //TIM_EncoderMode_TI12,
//			TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//
//	TIM_ICInitTypeDef TIM_ICInit_struct;
//	TIM_ICStructInit(&TIM_ICInit_struct);
//	TIM_ICInit_struct.TIM_Channel = TIM_Channel_1 || TIM_Channel_2 || TIM_Channel_3 || TIM_Channel_4;
//	TIM_ICInit_struct.TIM_ICFilter = 6;
//	//TIM_ICInit(TIM8, &TIM_ICInit_struct);
//
//	TIM_SetAutoreload(TIM8, 0xffff);
//
//	// turn on the timer/counters
//	TIM_Cmd(TIM8, ENABLE);
//	reset();
//	InitButton();
//}
//
//void QuadratureEncoder::InitButton(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;
//
//	//GPIO_StructInit(&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);
//
//	/* PC9 is connected to EXTI_Line9 */
//    EXTI_InitStruct.EXTI_Line = EXTI_Line9;
//    /* Enable interrupt */
//    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//    /* Interrupt mode */
//    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//    /* Triggers on rising and falling edge */
//    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
//    /* Add to EXTI */
//    EXTI_Init(&EXTI_InitStruct);
//
//    /* Add IRQ vector to NVIC */
//    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
//    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
//    /* Set priority */
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
//    /* Set sub priority */
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
//    /* Enable interrupt */
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//    /* Add to NVIC */
//    NVIC_Init(&NVIC_InitStruct);
//}
//
//bool QuadratureEncoder::hasButtonPressd()
//{
//	if (QuadratureEncoderInstance->button == true )
//		return true;
//	else
//		return false;
//}
//
//bool QuadratureEncoder::resetButtonPressd()
//{
//	button = false;
//}
//
//void QuadratureEncoder::reset(void) {
//	__disable_irq();
//	this->rotActual = 0;
//	this->rotTotal = 0;
//	//TIM_SetCounter(TIM8, 0);
//	//read();
//	__enable_irq();
//}
//
//void QuadratureEncoder::read(void) {
//	rotOld = rotActual;
//	rotActual = TIM_GetCounter(TIM8);
//	rotCount = rotActual - rotOld;
//	rotTotal += rotCount;
//}
//
//int32_t QuadratureEncoder::getRotaryPos(void) {
//	read();
//	return rotTotal;
//}
//
//int16_t QuadratureEncoder::getRotaryDiff(void) {
//	read();
//	return rotCount;
//}
//
//bool QuadratureEncoder::isRotDiff(void) {
//	if ( ((int16_t)TIM_GetCounter(TIM8)) != rotActual)
//		return true;
//	else
//		return false;
//}
//
///*
// *  Rotary Interrupt
// */
//extern "C" { void EXTI9_5_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line9) != RESET )
//  {
//	  QuadratureEncoderInstance->button=true;
//      EXTI_ClearITPendingBit(EXTI_Line9);
//  }
//
//}}
