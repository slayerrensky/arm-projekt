//#include "rtc.h"
//#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_rtc.h"
//#include "stm32f4xx_gpio.h"
//
//Rtc *RtcInstance;
//
//Rtc::Rtc(void){
//	Init();
//	InitWakeUpInterrupt(128);
//}
//
//void Rtc::Init()
//{
//    /* Enable the PWR clock */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//
//    /* Allow access to RTC */
//    PWR_BackupAccessCmd(ENABLE);
//
//    /* Enable the LSI OSC */
//    RCC_LSICmd(ENABLE);
//
//    /* Wait till LSI is ready */
//    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
//    {
//    }
//
//    /* Select the RTC Clock Source */
//    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//
//
//    /* Configure the RTC data register and RTC prescaler */
//    RTC_InitTypeDef RTC_InitStructure;
//    RTC_InitStructure.RTC_AsynchPrediv = 0x7F; // prescaler set to optain a 1 Hz signal of the 32kHz LSI
//    RTC_InitStructure.RTC_SynchPrediv = 0xFF; // prescaler set to optain a 1 Hz signal of the 32kHz LSI
//    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
//
//    /* Check on RTC init */
//    if (RTC_Init(&RTC_InitStructure) == ERROR)
//    {
//        return;
//    }
//
//    /* Enable the RTC Clock */
//    RCC_RTCCLKCmd(ENABLE);
//
//    /* Wait for RTC APB registers synchronisation */
//    RTC_WaitForSynchro();
//
//    rtc_InitWakeUpInterrupt(16);
//
//}
//
//void Rtc::InitWakeUpInterrupt(uint8_t wakeUpCounter)
//{
//    NVIC_InitTypeDef NVIC_InitStructure;
//    EXTI_InitTypeDef  EXTI_InitStructure;
//
//    // NVIC init
//    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//
//    // ext Interrupt 22 einstellen (fuer WakeUp)
//    EXTI_ClearITPendingBit(EXTI_Line22);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//
//    // zum einstellen muss Wakup disabled sein
//    RTC_WakeUpCmd(DISABLE);
//
//    // Teiler 16 => 32,768kHz:16 => 2048 Hz
//    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
//    // WakeUp Counter einstellen
//    RTC_SetWakeUpCounter(wakeUpCounter); //set to 16 -> 128 interrupts per second (32768Hz / Div16 = 2048; 2048 / 128 = 16
//
//    // enable Interrupt
//    RTC_ITConfig(RTC_IT_WUT, ENABLE);
//
//    // enable Wakeup
//    RTC_WakeUpCmd(ENABLE);
//}
//
//extern "C" {void RTC_WKUP_IRQHandler(void)
//{
//    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
//    {
//        // Interrupt Flags loeschen
//        EXTI_ClearITPendingBit(EXTI_Line22);
//        RTC_ClearITPendingBit(RTC_IT_WUT);
//
//
//        doSomething();
//
//    }
//}}
