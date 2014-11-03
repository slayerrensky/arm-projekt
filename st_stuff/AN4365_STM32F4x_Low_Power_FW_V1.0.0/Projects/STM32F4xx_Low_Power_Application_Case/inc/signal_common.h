/**
  ******************************************************************************
  * @file    STM32F4xx_Low_Power_Application_Case/inc/signal_common.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   This file contains all the functions prototypes signal_common.c
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
#ifndef __SIGNAL_COMMON_H
#define __SIGNAL_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup DSP_EXAMPLES
  * @{
  */

/** @addtogroup SIGNAL_COMMON
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef struct
{ 
  uint32_t process_init;
  uint32_t process;
  uint32_t process_min;
  uint32_t process_max;
  uint32_t process_average;
} PROCESS_ExecTime_TypeDef;
typedef struct 
{ 
  PROCESS_ExecTime_TypeDef FIR_Q15 ;
  PROCESS_ExecTime_TypeDef FastFIR_Q15;
  PROCESS_ExecTime_TypeDef FIR_Q31;
  PROCESS_ExecTime_TypeDef FastFIR_Q31;
  PROCESS_ExecTime_TypeDef FIR_F32;
  PROCESS_ExecTime_TypeDef FFT_LL_Q15;
  PROCESS_ExecTime_TypeDef FFT_BL_Q15;
  PROCESS_ExecTime_TypeDef FFT_LL_Q31; 
  PROCESS_ExecTime_TypeDef FFT_BL_Q31; 
  PROCESS_ExecTime_TypeDef FFT_LL_F32; 
  PROCESS_ExecTime_TypeDef FFT_BL_F32;
} Duration_TypeDef;
/* Exported constants --------------------------------------------------------*/
extern Duration_TypeDef DSP_Algo_Duration;
/* Exported macro ------------------------------------------------------------*/
/* Systick Start */
#define TimerCount_Start()	do{\
							SysTick->LOAD  =  0xFFFFFF  ;	/* set reload register */\
							SysTick->VAL  =  0  ;			/* Clear Counter */		 \
							SysTick->CTRL  =  0x5 ;			/* Enable Counting*/	 \
							}while(0)

/* Systick Stop and retrieve CPU Clocks count */								
#define TimerCount_Stop(Value) 	do {\
								Value = SysTick->VAL;/* Load the SysTick Counter Value */\
								SysTick->CTRL  =0;	/* Disable Counting */				 \
								Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
								}while(0)
/* Exported functions ------------------------------------------------------- */
ErrorStatus COMMON_TIM_Prescaler(uint32_t tim_pclk, uint32_t target_freq, uint16_t * pres, uint16_t *period);
uint32_t COMMON_getMin(uint32_t param1, uint32_t param2);
uint32_t COMMON_getMax(uint32_t param1, uint32_t param2);

#ifdef __cplusplus
}
#endif

#endif /* __SIGNAL_COMMON_H*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
