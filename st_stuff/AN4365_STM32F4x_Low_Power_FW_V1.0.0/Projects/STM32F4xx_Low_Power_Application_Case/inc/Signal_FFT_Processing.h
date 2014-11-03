/**
  ******************************************************************************
  * @file    STM32F4xx_Low_Power_Application_Case/inc/Signal_FFT_Processing.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   This file contains all the functions prototypes for the FFT processing
  *          firmware library.
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
#ifndef __SIGNAL_FFT_PROCESSING_H
#define __SIGNAL_FFT_PROCESSING_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "signal_common.h"

/** @addtogroup DSP_EXAMPLES
  * @{
  */

/** @addtogroup SIGNAL_FFT_PROCESSING
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* FFT processing with Q15 data functions *************************************/
arm_status FFT_PROCESSING_Q15Init(uint32_t FFT_Length);	
void FFT_PROCESSING_Q15Process(q15_t * FFT_Q15_InOut, uint32_t FFT_Length);
void FFT_PROCESSING_Q15Magnitude(q15_t* FFT_Q15_InOut, uint32_t FFT_Length,
                                 q15_t* Q15_MagOutput);
q15_t FFT_PROCESSING_Q15MaxMagValue(q15_t* FFT_Q15_Magnitude, uint32_t FFT_Length);
uint32_t FFT_PROCESSING_Q15MaxMagIndex(q15_t* FFT_Q15_Magnitude, uint32_t FFT_Length);

/* FFT processing with Q31 data functions *************************************/
arm_status FFT_PROCESSING_Q31Init(uint32_t FFT_Length);
void FFT_PROCESSING_Q31Process(q31_t * FFT_Q31_InOut, uint32_t FFT_Length);
void FFT_PROCESSING_Q31Magnitude(q31_t* FFT_Q31_InOut, uint32_t FFT_Length,
                                 q31_t* Q31_MagOutput); 
q31_t FFT_PROCESSING_Q31MaxMagValue(q31_t* FFT_Q31_Magnitude, uint32_t FFT_Length);
uint32_t FFT_PROCESSING_Q31MaxMagIndex(q31_t* FFT_Q31_Magnitude, uint32_t FFT_Length);

/* FFT processing with F32 data functions *************************************/
arm_status FFT_PROCESSING_F32Init(uint32_t FFT_Length);
void FFT_PROCESSING_F32Process(float32_t * FFT_F32_InOut, uint32_t FFT_Length);
void FFT_PROCESSING_F32Magnitude(float32_t* FFT_F32_InOut, uint32_t FFT_Length,
                                 float32_t* F32_MagOutput); 
float32_t FFT_PROCESSING_F32MaxMagValue(float32_t* FFT_F32_Magnitude, uint32_t FFT_Length);
uint32_t FFT_PROCESSING_F32MaxMagIndex(float32_t* FFT_F32_Magnitude, uint32_t FFT_Length);

#ifdef __cplusplus
}
#endif

#endif /* __SIGNAL_FFT_PROCESSING_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
