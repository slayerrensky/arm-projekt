/**
  ******************************************************************************
  * @file    STM32F4xx_Current_Consumption_Measuring/src/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Main program body
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
#include "main.h"
#include "pwr_modes.h"

/** @addtogroup STM32F4xx current consumption
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay = 0x00;
__IO uint32_t uwCounter = 0x00;
__IO uint8_t UserButtonStatus = 0x00;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
#if defined (RUN_MODE) || defined (SLEEP_MODE) || defined (STOP_MODE)
  static void Mode_Exit (void);
#endif /* (RUN_MODE) || (SLEEP_MODE) || (STOP_MODE) */
static void LedsConfig (void);

/* Private functions ---------------------------------------------------------*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  /*Configure Leds GPIOs */
  LedsConfig();

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

  /*** Execute selected power modes to measure relative current conusmption ***/

  /* Execte defined Run modes */
  Measure_Run ();
  /* Execute defined Sleep modes */
  Measure_Sleep ();
  /* Execute defined Stop modes */
  Measure_Stop ();
  /* Execute defined Standby modes */
  Measure_Standby ();

  /* Infinite loop */
  while (1)
  {}

}

/**
* @brief  Configure the Run modes exection routines.
* @param  None
* @retval None
*/
void Measure_Run (void)
{

#if defined (RUN_MODE)

#if defined (STM32F429_439xx) || defined (STM32F40_41xxx)

#if defined (RunPeriphDisabled180Mhz) && defined (STM32F429_439xx) && defined (VDD3_3)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_1);

  /*Enter Power mode */
  PWR_RunPeriphDisabled180Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphDisabled180Mhz && STM32F429_439xx */

#if defined (RunPeriphEnabled180Mhz) && defined (STM32F429_439xx) && defined (VDD3_3)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_2);

  /*Enter Power mode */
  PWR_RunPeriphEnabled180Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled180Mhz && defined STM32F429_439xx */

#if defined (RunPeriphDisabled168Mhz) || defined (RunPeriphDisabled160Mhz)

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_3);

  /*Enter Power mode */
  PWR_RunPeriphDisabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD3_3) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_1);

  /*Enter Power mode */
  PWR_RunPeriphDisabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD1_8) /* not STM32F40_41xxx && VDD3_3 */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_1);

  /*Enter Power mode */
  PWR_RunPeriphDisabled160Mhz();

#endif /* STM32F429_439xx */

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphDisabled168Mhz || RunPeriphDisabled160Mhz */

#if defined (RunPeriphEnabled168Mhz) || defined (RunPeriphEnabled160Mhz)

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_4);

  /*Enter Power mode */
  PWR_RunPeriphEnabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD3_3) /* not STM32F429_439xx*/

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_2);

  /*Enter Power mode */
  PWR_RunPeriphEnabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD1_8) /* not STM32F40_41xxx && VDD3_3 */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_2);

  /*Enter Power mode */
  PWR_RunPeriphEnabled160Mhz();

#endif /* (STM32F429_439xx) */

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled168Mhz || RunPeriphEnabled160Mhz */

#if defined RunPeriphDisabled144Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_5);

#elif defined (STM32F40_41xxx)  /* not STM32F429_439xx */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_3);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_RunPeriphDisabled144Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphDisabled144Mhz */

#if defined RunPeriphEnabled144Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_6);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx*/

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_4);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_RunPeriphEnabled144Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled144Mhz */

#if defined RunPeriphDisabled120Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_7);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_5);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_RunPeriphDisabled120Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /*  RunPeriphDisabled120Mhz */

#if defined RunPeriphEnabled120Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_8);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_6);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_RunPeriphEnabled120Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled120Mhz*/

#elif defined (STM32F401xx)  /* Not  STM32F429_439xx || STM32F40_41xxx */

#if defined (RunPeriphDisabled84Mhz)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_1);

  /*Enter Power mode */
  PWR_RunPeriphDisabled84Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphDisabled84Mhz */

#if defined (RunPeriphEnabled84Mhz)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_2);

  /*Enter Power mode */
  PWR_RunPeriphEnabled84Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled84Mhz */

#if defined (RunPeriphDisabled60Mhz)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_3);

  /*Enter Power mode */
  PWR_RunPeriphDisabled60Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphDisabled60Mhz */

#if defined (RunPeriphEnabled60Mhz)

  /* Wait User push buttom to enter Run mode */
  WaitUser_PushBottom (MODE_1, SUB_MODE_4);

  /*Enter Power mode */
  PWR_RunPeriphEnabled60Mhz();

  /* Perform CoreMark */
  main_core();

  /*Exit from previous mode*/
  Mode_Exit ();

#endif /* RunPeriphEnabled60Mhz */

#endif /* STM32F429_439xx) || STM32F40_41xxx */

#endif /* RUN_MODE */

}


/**
* @brief  Configure the Sleep modes exection routines.
* @param  None
* @retval None
*/
void Measure_Sleep (void)
{

#if defined (SLEEP_MODE)

#if defined (STM32F429_439xx) || defined (STM32F40_41xxx)

#if defined (SleepPeriphDisabled180Mhz) && defined (STM32F429_439xx) && defined (VDD3_3)

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_1);

  PWR_SleepPeriphDisabled180Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled180Mhz && STM32F429_439xx */

#if defined (SleepPeriphEnabled180Mhz) && defined (STM32F429_439xx) && defined (VDD3_3)

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_2);

  PWR_SleepPeriphEnabled180Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled180Mhz && STM32F429_439xx */

#if defined (SleepPeriphDisabled168Mhz) || defined (SleepPeriphDisabled160Mhz)

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_3);

  /*Enter Power mode */
  PWR_SleepPeriphDisabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD3_3) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_1);

  /*Enter Power mode */
  PWR_SleepPeriphDisabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD1_8) /* not STM32F40_41xxx && VDD3_3 */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_1);

  /*Enter Power mode */
  PWR_SleepPeriphDisabled160Mhz();

#endif /* STM32F429_439xx */

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled168Mhz || SleepPeriphDisabled160Mhz */

#if defined (SleepPeriphEnabled168Mhz) || defined (SleepPeriphEnabled160Mhz)

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_4);
  /*Enter Power mode */
  PWR_SleepPeriphEnabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD3_3) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_2);

  /*Enter Power mode */
  PWR_SleepPeriphEnabled168Mhz();

#elif defined (STM32F40_41xxx) && defined (VDD1_8) /* STM32F40_41xxx && VDD3_3 */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_2);

  /*Enter Power mode */
  PWR_SleepPeriphEnabled160Mhz();

#endif /* STM32F429_439xx */

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled168Mhz || SleepPeriphEnabled160Mhz */



#if defined SleepPeriphDisabled144Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_5);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_3);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_SleepPeriphDisabled144Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled144Mhz */

#if defined SleepPeriphEnabled144Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_6);

#elif defined (STM32F40_41xxx) /* STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_4);

#endif /* STM32F429_439xx */

  /*Enter Power mode */
  PWR_SleepPeriphEnabled144Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled144Mhz */

#if defined SleepPeriphDisabled120Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_7);

#elif defined (STM32F40_41xxx) /* STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_5);

#endif /* STM32F429_439xx */


  PWR_SleepPeriphDisabled120Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled120Mhz */

#if defined SleepPeriphEnabled120Mhz

#if defined (STM32F429_439xx)

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_8);

#elif defined (STM32F40_41xxx) /* not STM32F429_439xx */

  /* Wait User push buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_6);

#endif /* STM32F429_439xx */

  PWR_SleepPeriphEnabled120Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled120Mhz */

#elif defined (STM32F401xx)  /* Not  STM32F429_439xx || STM32F40_41xxx */

#if defined SleepPeriphDisabled84Mhz

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_1);

  PWR_SleepPeriphDisabled84Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled84Mhz */

#if defined SleepPeriphEnabled84Mhz

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_2);

  PWR_SleepPeriphEnabled84Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled84Mhz */

#if defined SleepPeriphDisabled60Mhz

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_3);

  PWR_SleepPeriphDisabled60Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphDisabled60Mhz */

#if defined SleepPeriphEnabled60Mhz

  /* Wait User puch buttom to enter Sleep mode */
  WaitUser_PushBottom (MODE_2, SUB_MODE_3);

  PWR_SleepPeriphEnabled60Mhz();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* SleepPeriphEnabled60Mhz */

#endif /* STM32F429_439xx || STM32F40_41xxx */

#endif /* SLEEP_MODE */
}


/**
* @brief  Configure the Stop modes exection routines.
* @param  None
* @retval None
*/
void Measure_Stop (void)
{

#if defined (STOP_MODE)

#if defined StopMainRegFlashStop

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_1);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopMainRegFlashStop();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopMainRegFlashStop */

#if defined StopMainRegFlashPwrDown

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_2);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopMainRegFlashPwrDown ();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopMainRegFlashPwrDown */

#if defined StopLowPwrRegFlashStop

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_3);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopLowPwrRegFlashStop ();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopLowPwrRegFlashStop */

#if defined StopLowPwrRegFlashPwrDown

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_4);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopLowPwrRegFlashPwrDown ();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopLowPwrRegFlashPwrDown */

#if defined StopMainRegUnderDriveFlashPwrDown

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_5);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopMainRegUnderDriveFlashPwrDown ();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopMainRegUnderDriveFlashPwrDown */

#if defined StopLowPwrRegUnderDriveFlashPwrDown

  /* Wait User puch buttom to enter Stop mode */
  WaitUser_PushBottom (MODE_3, SUB_MODE_6);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StopLowPwrRegUnderDriveFlashPwrDown ();

  /* Exit from sleep mode  */
  Mode_Exit ();

#endif /* StopLowPwrRegUnderDriveFlashPwrDown */

#endif /* STOP_MODE */

}

/**
* @brief  Configure the Standby modes exection routines.
* @param  None
* @retval None
*/
void Measure_Standby (void)
{

#if defined (STANDBY_MODE)

#if defined StandbyBkpSramOnRtcON

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_1);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StandbyBkpSramOnRtcON ();

#endif /* StandbyBkpSramOnRtcON */

#if defined StandbyBkpSramOffRtcOn

#if defined (STM32F429_439xx) || defined (STM32F40_41xxx)

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_2);

#elif defined (STM32F401xx)

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_1);

#endif

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StandbyBkpSramOffRtcOn ();

#endif /* StandbyBkpSramOffRtcOn */

#if defined StandbyBkpSramOnRtcOff

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_3);

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StandbyBkpSramOnRtcOff ();

#endif /* StandbyBkpSramOnRtcOff */

#if defined StandbyBkpSramOffRtcOff

#if defined (STM32F429_439xx) || defined (STM32F40_41xxx)

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_4);

#elif defined (STM32F401xx)

  /* Wait User puch buttom to enter Standby mode */
  WaitUser_PushBottom (MODE_4, SUB_MODE_2);

#endif

  /* Configure CPIOs as analog inputs */
  GPIO_AnalogConfig ();

  PWR_StandbyBkpSramOffRtcOff ();

#endif /* StandbyBkpSramOffRtcOff */

#endif /* STANDBY_MODE */

}


/**
* @brief  Configure All GPIO as analog input.
* @param  None
* @retval None
*/
void GPIO_AnalogConfig (void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Configure GPIOs as Analog input to reduce current consumption*/
  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                          RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                          RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH , ENABLE);

#ifndef STM32F401xx

  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | \
                          RCC_AHB1Periph_GPIOI, ENABLE);

#ifdef STM32F429_439xx

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, ENABLE);

#endif /* STM32F429_439xx */

#endif /* not STM32F401xx*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
#if defined (STM32F429_439xx) || defined (STM32F40_41xxx)
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_Init(GPIOI, &GPIO_InitStructure);
#if defined STM32F429_439xx
  GPIO_Init(GPIOJ, &GPIO_InitStructure);
  GPIO_Init(GPIOK, &GPIO_InitStructure);
#endif /* STM32F429_439xx */
#endif /* STM32F429_439xx || STM32F40_41xxx */
#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif /* not DEBUG */

  /* Disable GPIOs clock */
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                          RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                          RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH , DISABLE);

#ifndef STM32F401xx
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | \
                          RCC_AHB1Periph_GPIOI, DISABLE);

#ifdef STM32F429_439xx
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, DISABLE);
#endif /* STM32F429_439xx */
#endif /* not STM32F401xx */
}


/**
* @brief  Configure GPIOs for Led3 and Led 4.
* @param  None
* @retval None
*/
static void LedsConfig (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /*Configure GPIO structure */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

#if defined (STM32F429_439xx)

  /* Enable the GPIO_LED Port G Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);

  /* Configure the GPIO_LED 3 pin PG13 and GPIO_LED 4 pin PG14*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_SetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* not STM32F429_439xx */

  /* Enable the GPIO_LED Port G Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);

  /* Configure the GPIO_LED 3 pin PD12 and the GPIO_LED 4 pin PD14 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_12;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14);
#endif /* STM32F429_439xx */
}


/**
* @brief  Configure the PA0 pin as interrupt wakeup source
* @param  None
* @retval None
*/
void WakeUpPinInt_configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI Line0 */
  EXTI_DeInit();
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
* @brief  Configure power mode switch routine.
* @param  mode: specifies the power mode that we are currently measuring.
*          This parameter can be one of the following values:
*            @arg MODE_1: Run mode
*            @arg MODE_2: STOP mode
*            @arg MODE_3: STOP mode
*            @arg MODE_4: Standby mode
* @param  submode: specifies which submode that we are currently measuring
*          This parameter can be one of the following values:
*            @arg SUB_MODE_1: submode numero 1
*            @arg SUB_MODE_2: submode numero 2
*            @arg SUB_MODE_3: submode numero 3
*            @arg SUB_MODE_4: submode numero 4
*            @arg SUB_MODE_5: submode numero 5
*            @arg SUB_MODE_6: submode numero 6
*            @arg SUB_MODE_7: submode numero 7
*            @arg SUB_MODE_8: submode numero 8

* @retval None
*/

void WaitUser_PushBottom (uint8_t mode, uint8_t submode)
{
  uint8_t mode_counter = 0;
  uint8_t submode_counter = 0;
  UserButtonStatus = RESET;

  /* Wait until user push-button is pressed to enter the Low Power mode */
  while (UserButtonStatus == RESET)
  {

    /*Highlight green and RED LEDs to indicate begin a power mode consumption */
#if defined (STM32F429_439xx)

    GPIO_ToggleBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);

#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* not STM32F429_439xx  */

    GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14);

#endif /* STM32F429_439xx */

  }

#if defined (STM32F429_439xx)

  GPIO_ResetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);

#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* not STM32F429_439xx */

  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14);

#endif /* STM32F429_439xx */


  /* Loop while user push-button is maintained pressed */
  while ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) != RESET)
  {}

  for (mode_counter = 0; mode_counter < mode ; mode_counter ++)
  {
    /* Toggle The LED4 */
#if defined (STM32F429_439xx)

    /* Turn off LED4 */
    GPIO_ResetBits(GPIOG, GPIO_Pin_14);

    /* Inserted Delay */
    Delay(40);

    /* Turn on LED4 */
    GPIO_SetBits(GPIOG, GPIO_Pin_14);

#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* not STM32F429_439xx */

    /* Turn off LED4 */
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);

    /* Inserted Delay */
    Delay(40);

    /* Turn on LED4 */
    GPIO_SetBits(GPIOD, GPIO_Pin_14);

#endif /* STM32F429_439xx */

    /* Inserted Delay */
    Delay(80);
  }

  /* Inserted Delay */
  Delay(30);


#if defined (STM32F429_439xx)

  /* Turn off LED4 */
  GPIO_ResetBits(GPIOG, GPIO_Pin_14);

#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* STM32F429_439xx */

  /* Turn off LED4 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_14);

#endif /* STM32F429_439xx  */


  for (submode_counter = 0; submode_counter < submode ; submode_counter ++)
  {
    /* Toggle The LED3 */
#if defined (STM32F429_439xx)

    /* Turn off LED3 */
    GPIO_ResetBits(GPIOG, GPIO_Pin_13);

    /* Inserted Delay */
    Delay(40);

    /* Turn on LED3 */
    GPIO_SetBits(GPIOG, GPIO_Pin_13);

#elif defined (STM32F40_41xxx) || defined (STM32F401xx) /* not STM32F429_439xx */

    /* Turn off LED3 */
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);

    /* Inserted Delay */
    Delay(40);

    /* Turn on LED3 */
    GPIO_SetBits(GPIOD, GPIO_Pin_12);

#endif /* STM32F429_439xx */

    /* Inserted Delay */
    Delay(80);

  }
}

#if defined (RUN_MODE) || defined (SLEEP_MODE) || defined (STOP_MODE)
/**
* @brief  Configure the exit routine from power mode.
* @param  None
* @retval None
*/
static void Mode_Exit (void)
{
  /* Clock init configuration */
  RCC_DeInit();

  /* Disable HSE */
  RCC_HSEConfig(RCC_HSE_OFF);

  /* Enable HSI */
  RCC_HSICmd(ENABLE);

  /* Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}

  /* Select HSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

  /* Wait till HSI is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x00)
  {}

  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /*Configure Leds GPIOs */
  LedsConfig();

  /* Configure Wakeup pin  */
  WakeUpPinInt_configuration();

}
#endif /* RUN_MODE) || (SLEEP_MODE) || (STOP_MODE) */

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  /* Returns the frequencies of different on chip clocks */
  RCC_GetClocksFreq(&RCC_Clocks);

  /*-----------------------Set systick config --------------------*/
  SysTick->CTRL = 0004;
  /* set reload register */
  SysTick->LOAD  = ((RCC_Clocks.HCLK_Frequency / 1000) * nTime) - 1;
  /* set Priority for Systick Interrupt */
  NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
  /* Load the SysTick Counter Value */
  SysTick->VAL   = 0;
  /* Enable SysTick counter  */
  SysTick->CTRL  =  SysTick_CTRL_ENABLE_Msk;
  /*-------------------------------------------------------------*/
  /* Loop until counter reach 0 */
  while (((SysTick->CTRL)& 0x10000) != 0x10000);
  SysTick->CTRL = 0004; /* Set systick to default config */

}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  return ch;
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
