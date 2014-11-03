/**
  ******************************************************************************
  * @file    STM32F4xx_Current_Consumption_Measuring/src/pwr_modes.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Power modes functions 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/* CODE & PRODUCT CONFIGURATION ----------------------------------------------*/
#include "stm32f4xx.h"
#include "pwr_modes.h"
#include "main.h"

/** @addtogroup STM32F4xx current consumption
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WakeupCounter 0xA000 /*specifies the WakeUp counter*/
#define HSE_DISCO 8

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
GPIO_InitTypeDef GPIO_InitStructure;


/*****************  STOP mode function ****************************************/

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Main mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in Stop mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopMainRegFlashStop (void)
{

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Disable FLASH Deep Power Down Mode by clearing FPDS bit*/
  PWR_FlashPowerDownCmd(DISABLE);

  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Main mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopMainRegFlashPwrDown (void)
{
  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* FLASH Deep Power Down Mode enabled */
  PWR_FlashPowerDownCmd(ENABLE);

  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Low Power mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in STOP mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopLowPwrRegFlashStop (void)
{
  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Disable FLASH Deep Power Down Mode by clearing FPDS bit*/
  PWR_FlashPowerDownCmd(DISABLE);

  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Low Power mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopLowPwrRegFlashPwrDown (void)
{
  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* FLASH Deep Power Down Mode enabled */
  PWR_FlashPowerDownCmd(ENABLE);

  /* Enter Stop Mode Reg LP*/
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

}

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Main & under drive mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopMainRegUnderDriveFlashPwrDown (void)
{

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /*Enables the Under-Drive mode. */
  PWR_UnderDriveCmd(ENABLE);

  /* Enter Stop Mode  low voltage*/
  PWR_EnterUnderDriveSTOPMode(PWR_MainRegulator_UnderDrive_ON , PWR_STOPEntry_WFI);
}

/**
  * @brief  This function configures the system to enter Stop mode
  *         for current consumption measurement purpose.
  *         ===========================================
  *           - Regulator in Low Power & under drive mode
  *           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
  *           - No IWDG
  *           - FLASH in deep power down mode
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StopLowPwrRegUnderDriveFlashPwrDown (void)
{

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /*Enables the Under-Drive mode. */
  PWR_UnderDriveCmd(ENABLE);

  /* Enter Stop Mode  low voltage*/
  PWR_EnterUnderDriveSTOPMode(PWR_LowPowerRegulator_UnderDrive_ON, PWR_STOPEntry_WFI);
}

/************************  STANDBY mode function ******************************/
/**
  * @brief  This function configures the system to enter Standby mode with RTC
  *         clocked by LSI and with Backup SRAM ON for current consumption
  *         measurement purpose.
  *         STANDBY Mode with RTC clocked by LSI and BKPSRAM ON
  *         ===================================================
  *           - RTC Clocked by LSI
  *           - Backup SRAM ON
  *           - IWDG OFF
  *           - Automatic Wakeup using RTC clocked by LSI (after ~20s)
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StandbyBkpSramOnRtcON (void)
{
  /* Allow access to the backup domain (RTC registers, RTC
     backup data registers and backup SRAM) */
  PWR_BackupAccessCmd(ENABLE);

  /* LSI used as RTC source clock*/

  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /*-------------------- Backup SRAM -------------------*/

  /* Enable BKPRAM Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

  /* Enable the Backup SRAM low power Regulator */
  PWR_BackupRegulatorCmd(ENABLE);

  /* Wait until the Backup SRAM low power Regulator is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
  {}
  /*-----------------------------------------------------*/

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~20s
      RTC Clock Source LSI ~32KHz 

      Wakeup Time Base = (16 / LSI) * WakeUpCounter
   */

  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(WakeupCounter);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);

  /* Clear WakeUp (WUTF) pending flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Request to enter STANDBY mode*/
  PWR_EnterSTANDBYMode();

}

/**
 * @brief  This function configures the system to enter Standby mode with Backup SRAM ON
 *         for current consumption measurement purpose.
 *         STANDBY Mode with BKPSRAM ON and RTC OFF
 *         ========================================
 *           - Backup SRAM ON RTC OFF
 *           - IWDG and LSI OFF
 *           - Wakeup using WakeUp Pin (PA.00)
 * @param  None
 * @retval None
 */
void PWR_StandbyBkpSramOnRtcOff (void)
{

  /* Allow access to the backup domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Enable BKPRAM Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

  /* Enable the Backup SRAM low power Regulator */
  PWR_BackupRegulatorCmd(ENABLE);

  /* Wait until the Backup SRAM low power Regulator is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
  {}

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Request to enter STANDBY mode*/
  PWR_EnterSTANDBYMode();

}


/**
  * @brief  This function configures the system to enter Standby mode with RTC
  *         clocked by LSI for current consumption measurement purpose.
  *         STANDBY Mode with RTC clocked by LSI
  *         ====================================
  *           - RTC Clocked by LSI
  *           - IWDG OFF
  *           - Backup SRAM OFF
  *           - Automatic Wakeup using RTC clocked by LSI (after ~20s)
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StandbyBkpSramOffRtcOn (void)
{
  /* Allow access to the backup domain (RTC registers, RTC
  *     backup data registers and backup SRAM) */
  PWR_BackupAccessCmd(ENABLE);

  /* LSI used as RTC source clock*/
  /* The RTC Clock may varies due to LSI frequency dispersion. */

  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~20s
     RTC Clock Source LSI ~32KHz 

     Wakeup Time Base = (16 / LSI) * WakeUpCounter
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(WakeupCounter);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Clear WakeUp (WUTF) pending flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Request to enter STANDBY mode */
  PWR_EnterSTANDBYMode();

}

/**
  * @brief  This function configures the system to enter Standby mode for
  *         current consumption measurement purpose.
  *         STANDBY Mode
  *         ============
  *           - Backup SRAM and RTC OFF
  *           - IWDG and LSI OFF
  *           - Wakeup using WakeUp Pin (PA.00)
  * @param  None
  * @retval None
  */
void PWR_StandbyBkpSramOffRtcOff (void)
{
  /* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);

  /* Clear Wakeup flag*/
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable WKUP pin 1 */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter STANDBY mode*/
  PWR_EnterSTANDBYMode();
}

/************************** RUN mode functions  *******************************/

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 180MHz All Peripherals Disabled
  *         ========================================
  *            - System Running at PLL (180MHz)
  *            - Over drive scale 1
  *            - Flash 5 wait state
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals disabled.
  *
  * @note  This mode is available only for STM32F427_437xx device
  * @note  This mode is available only with 3,3 V in this project
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled180Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif /* VDD3_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /*---------------over drive config -------------*/

    PWR_OverDriveCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODRDY) == RESET)
    {}

    PWR_OverDriveSWCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) == RESET)
    {}

    /*----------------------------------------------*/

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (360 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif /* not STM32F401xx */

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 180MHz All Peripherals Enabled
  *         =======================================
  *            - System Running at PLL (180MHz)
  *            - Over drive scale 1
  *            - Flash 5 wait state
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is available only for STM32F427_437xx device
  * @note  This mode is available only with 3,3 V in this project
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled180Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif /* VDD3_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /*--------------- over drive config -------------*/

    PWR_OverDriveCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODRDY) == RESET)
    {}

    PWR_OverDriveSWCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) == RESET)
    {}

    /*-----------------------------------------------*/

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (360 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */

#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 168 MHz ALL Peripherals Disabled
  *         =========================================
  *            - System Running at PLL (168MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 8 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @note  This mode is not available for STM32F407 device when we work
  * with 1,8 v as power supply. We can perform up to 160 MHz
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled168Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /*Select scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 8 wait state */
    FLASH_SetLatency(FLASH_Latency_8);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 168 MHz ALL Peripherals Enabled
  *         =========================================
  *            - System Running at PLL (168MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 8 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @note  This mode is not available for STM32F407 device when we work
  * with 1,8 v as power supply. We can perform up to 160 MHz
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled168Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /*Select scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 8 wait state */
    FLASH_SetLatency(FLASH_Latency_8);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);
}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 160 MHz ALL Peripherals Disabled
  *         ==========
  *            - System Running at PLL (160MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 8 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *
  * @note  This mode is not available for STM32F40_41xxx devices
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled160Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /*Select scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (320 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);
}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 160 MHz ALL Peripherals Enabled
  *         ========================================
  *            - System Running at PLL (160MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled160Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /*Select scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (320 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);
}


/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 144 MHz ALL Peripherals Disabled
  *         =========================================
  *            - System Running at PLL (144MHz)
  *            - scale 1
  *            - Flash :  4 wait state(3,3V) /  7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled144Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
#if defined (STM32F429_439xx)
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (288 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 144 MHz ALL Peripherals Enabled
  *         =========================================
  *            - System Running at PLL (144MHz)
  *            - scale 1
  *            - Flash :  4 wait state(3,3V) /  7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled144Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
#if defined (STM32F429_439xx)
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (288 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

}

/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 120 MHz ALL Peripherals Disabled
  *         =========================================
  *            - System Running at PLL (120MHz)
  *            - scale 1
  *            - Flash :   3 wait state(3,3V) /  5 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled120Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
#if defined (STM32F429_439xx)
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);
#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);
#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 3 wait state */
    FLASH_SetLatency(FLASH_Latency_3);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}


    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}


/**
  * @brief  This function configures the system to enter RUN mode
  *         for current consumption measurement purpose.
  *         RUN Mode 120 MHz ALL Peripherals Enabled
  *         =========================================
  *            - System Running at PLL (120MHz)
  *            - scale 1
  *            - Flash :   3 wait state(3,3V) /  5 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled120Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
#if defined (STM32F429_439xx)
    /*Select scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);
#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /*Select scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);
#endif /* STM32F429_439xx */

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3

    /* Flash 3 wait state */
    FLASH_SetLatency(FLASH_Latency_3);

#elif defined VDD1_8 /* not VDD1_3 */

    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif /* VDD1_3 */

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

}


/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 180MHz All Peripherals Disabled
  *         ==========================================
  *            - System Running at PLL (180MHz)
  *            - Over drive scale 1
  *            - Flash 5 wait state
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - All peripherals disabled.
  *            - using the external 8 MHz oscillator
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F427_437xx device
  * @note  This mode is available only with 3,3 V in this project
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled180Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /*---------------over drive config --------------*/

    PWR_OverDriveCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODRDY) == RESET)
    {}

    PWR_OverDriveSWCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) == RESET)
    {}

    /*-----------------------------------------------*/

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (360 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 180MHz All Peripherals Enabled
  *         ==========================================
  *            - System Running at PLL (180MHz)
  *            - Over drive scale 1
  *            - Flash 5 wait state
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - All peripherals enabled.
  *            - using the external 8 MHz oscillator
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F427_437xx device
  * @note  This mode is available only with 3,3 V in this project
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled180Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 1 mode */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /*---------------- over drive config -----------*/

    PWR_OverDriveCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODRDY) == RESET)
    {}

    PWR_OverDriveSWCmd(ENABLE);

    while ( PWR_GetFlagStatus(PWR_FLAG_ODSWRDY) == RESET)
    {}

    /*-----------------------------------------------*/

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (360 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 168 MHz ALL Peripherals Disabled
  *         ===========================================
  *            - System Running at PLL (168MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 8 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @note  This mode is not available for STM32F407 device when we work
 *     with 1,8 v as power supply. We can perform up to 160 MHz
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled168Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8
    /* Flash 8 wait state */
    FLASH_SetLatency(FLASH_Latency_8);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 168 MHZ All peripherals Enabled
  *         =========================================
  *            - System Running at PLL (168MHz)
  *            - scale 1
  *            - Flash : 5 wait state(3,3V) / 8 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @note  This mode is not available for STM32F407 device when we work
 *     with 1,8 v as power supply. We can perform up to 160 MHz
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled168Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8
    /* Flash 8 wait state */
    FLASH_SetLatency(FLASH_Latency_8);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 160 MHz ALL Peripherals Disabled
  *         ============================================
  *            - System Running at PLL (160MHz)
  *            - scale 1
  *            - Flash : 5 wait state (3,3v) / 7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled160Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8
    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (320 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 160 MHz ALL Peripherals Enabled
  *         ==========================================
  *            - System Running at PLL (160MHz)
  *            - scale 1
  *            - Flash : 5 wait state (3,3v) / 7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled160Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 1 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#elif defined VDD1_8
    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (320 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 144 MHz ALL Peripherals Disabled
  *         ===========================================
  *            - System Running at PLL (144MHz)
  *            - scale 1
  *            - Flash :  4 wait state(3,3V) /  7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled144Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);

#elif defined VDD1_8
    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (288 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */

#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 144 MHz ALL Peripherals Enabled
  *         ==========================================
  *            - System Running at PLL (144MHz)
  *            - scale 1
  *            - Flash :  4 wait state(3,3V) /  7 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals enabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled144Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);

#elif defined VDD1_8
    /* Flash 7 wait state */
    FLASH_SetLatency(FLASH_Latency_7);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (288 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 120 MHz ALL Peripherals Disabled
  *         =========================================
  *            - System Running at PLL (120MHz)
  *            - scale 1
  *            - Flash :   3 wait state(3,3V) /  5 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled120Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 3 wait state */
    FLASH_SetLatency(FLASH_Latency_3);

#elif defined VDD1_8
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system to enter SLEEP mode
  *         for current consumption measurement purpose.
  *         SLEEP Mode 120 MHz ALL Peripherals Enabled
  *         ==========================================
  *            - System Running at PLL (120MHz)
  *       - scale 1
  *            - Flash :   3 wait state(3,3V) /  5 wait state(1,8V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
 *       - using the external 8 MHz oscillator
  *            - All peripherals enabled.
 *      - Wakeup using WakeUp Pin (PA.00)
 *
  * @note  This mode is not available for STM32F401xx device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled120Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale mode */
#if defined (STM32F429_439xx)
    /* Select Scale 3 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/
    /* Select Scale 2 */
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#endif /* STM32F429_439xx */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 3 wait state */
    FLASH_SetLatency(FLASH_Latency_3);

#elif defined VDD1_8
    /* Flash 5 wait state */
    FLASH_SetLatency(FLASH_Latency_5);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div2);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div4);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((2 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/

  /* GPIOs Periph clock enable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 84 MHz All Peripherals Disabled
  *         ========================================
  *            - System Running at PLL (84MHz)
  *            - Scale 2
  *            - Flash: 2 wait state (3,3 V) / 4 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals disabled.
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled84Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 2 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

#elif defined VDD1_8
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 84 MHz All Peripherals Enabled
  *         =======================================
  *            - System Running at PLL (84MHz)
  *            - scale 2
  *            - Flash : 2 wait state (3,3 V) / 4 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled84Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 2 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

#elif defined VDD1_8
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

}

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 60 MHz All Peripherals Disabled
  *         ========================================
  *            - System Running at PLL (60MHz)
  *            - Scale 3
  *            - Flash : 1 wait state (3,3 V) / 2 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals disabled.
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphDisabled60Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 3 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

#elif defined VDD1_8
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

}

/**
  * @brief  This function configures the system on RUN mode for
  *         current consumption measurement purpose.
  *         RUN Mode 60 MHz All Peripherals Enabled
  *         =======================================
  *            - System Running at PLL (60MHz)
  *            - Scale 3
  *            - Flash : 1 wait state (3,3 V) / 2 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals enabled.
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_RunPeriphEnabled60Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 3 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

#elif defined VDD1_8
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

}

/**
  * @brief  This function configures the system on Sleep mode for
  *         current consumption measurement purpose.
  *         SLEEP Mode 84 MHz All Peripherals Disabled
  *         ==========================================
  *            - System Running at PLL (84MHz)
  *            - Scale 2
  *            - Flash : 2 wait state (3,3 V) / 4 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled84Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 2 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

#elif defined VDD1_8
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system on SLEEP mode for
  *         current consumption measurement purpose.
  *         SLEEP Mode 84 MHz All Peripherals Enabled
  *         =========================================
  *            - System Running at PLL (84MHz)
  *            - Scale 2
  *            - Flash : 2 wait state (3,3 V) / 4 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals Enabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled84Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 2 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale2);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

#elif defined VDD1_8
    /* Flash 4 wait state */
    FLASH_SetLatency(FLASH_Latency_4);

#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (336 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* All Periph clock Enable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system on SLEEP mode for
  *         current consumption measurement purpose.
  *         SLEEP Mode 60 MHz All Peripherals Disabled
  *         =========================================
  *            - System Running at PLL (60MHz)
  *            - Scale 3
  *            - Flash : 1 wait state (3,3 V) / 2 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals disabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphDisabled60Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 3 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* Enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* Enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

#elif defined VDD1_8
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* GPIOs Periph clock disable */
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, DISABLE);

  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, DISABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, DISABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, DISABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, DISABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, DISABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/**
  * @brief  This function configures the system on SLEEP mode for
  *         current consumption measurement purpose.
  *         SLEEP Mode 60 MHz All Peripherals Enabled
  *         =========================================
  *            - System Running at PLL (60MHz)
  *            - Scale 3
  *            - Flash : 1 wait state (3,3 V) / 2 wait state (1,8 V)
  *            - Instruction and Data caches ON/OFF
  *            - Prefetch ON/OFF
  *            - Code running from Internal FLASH
  *            - using the external 8 MHz oscillator
  *            - All peripherals enabled.
  *            - Wakeup using WakeUp Pin (PA.00)
  *
  * @note  This mode is available only for STM32F401x device
  * @param  None
  * @retval None
  */

void PWR_SleepPeriphEnabled60Mhz(void)
{
  /* Clock init configuration ------------------------------------------------*/
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {

    /* Enable PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Select regulator voltage output Scale 3 mode*/
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale3);

#if defined ART_Enable

#if defined Prefetch_Enable

    /* Enable prefetch buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

#else /* Prefetch_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

#endif /* Prefetch_Enable */

    /* enable flash instruction cache */
    FLASH_InstructionCacheCmd(ENABLE);

    /* enable flash data cache */
    FLASH_DataCacheCmd(ENABLE);

#else /* not ART_Enable */

    /* Disable prefetch buffer */
    FLASH_PrefetchBufferCmd(DISABLE);

    /* Disable flash instruction cache */
    FLASH_InstructionCacheCmd(DISABLE);

    /* Disable flash data cache */
    FLASH_DataCacheCmd(DISABLE);

#endif /* ART_Enable */

    /* Configure Wait states according to power supply voltage */
#if defined VDD3_3
    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

#elif defined VDD1_8
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
#endif

    /* Configures the AHB clock */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK/2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/4 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* configure the PLL */
    RCC->PLLCFGR = HSE_DISCO | (240 << 6) | (((4 >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                   (7 << 24);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {}
  }

  /* GPIO CONFIG -------------------------------------------------------------*/
  RCC_AHB1PeriphClockCmd(ALL_GPIOs, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger
     OFF) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#ifndef STM32F401xx
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#ifdef STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */


  /* All Periph clock Disable */
  RCC_AHB1PeriphClockCmd(ALL_AHB1_PERIPH, ENABLE);

  RCC_AHB2PeriphClockCmd(ALL_AHB2_PERIPH, ENABLE);

#ifndef STM32F401xx
  RCC_AHB3PeriphClockCmd(ALL_AHB3_PERIPH, ENABLE);
#endif

  RCC_APB1PeriphClockCmd(ALL_APB1_PERIPH, ENABLE);

  RCC_APB2PeriphClockCmd(ALL_APB2_PERIPH, ENABLE);

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /* Enable the wakeup pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter SLEEP mode */
  __WFI();

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

