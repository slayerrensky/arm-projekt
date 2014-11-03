/**
  ******************************************************************************
  * @file    STM32F4xx_Current_Consumption_Measuring/inc/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "coremark.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#if !defined (RUN_MODE) && !defined (SLEEP_MODE) && !defined (STOP_MODE) && !defined (STANDBY_MODE)

   /* Uncomment the corresponding line to select Low Power mode(s) that will be measured */
  #define RUN_MODE   
  #define SLEEP_MODE
  #define STOP_MODE
  #define STANDBY_MODE   

#endif

#if !defined (RUN_MODE) && !defined (SLEEP_MODE) && !defined (STOP_MODE) && !defined (STANDBY_MODE)
 #error "Please select first the target STM32F4xx Low Power modes to be measured (main.h file)"
#endif


/* Uncommenting this macro enable to debug code by enabling JTAG pins*/
/* #define DEBUG */

/* Exported macro ------------------------------------------------------------*/

#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4
#define SUB_MODE_1 1
#define SUB_MODE_2 2
#define SUB_MODE_3 3
#define SUB_MODE_4 4
#define SUB_MODE_5 5
#define SUB_MODE_6 6
#define SUB_MODE_7 7
#define SUB_MODE_8 8

/* Exported functions ------------------------------------------------------- */
void WakeUpPinInt_configuration(void);
void WaitUser_PushBottom (uint8_t i1, uint8_t i2);
void GPIO_AnalogConfig (void);
void Measure_Run (void);
void Measure_Sleep (void);
void Measure_Stop (void);
void Measure_Standby (void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
MAIN_RETURN_TYPE main_core(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
