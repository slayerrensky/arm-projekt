/**
  ******************************************************************************
  * @file    STM32F4xx_Low_Power_Application_Case/src/main.c
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
#include "arm_math.h"  
#include "arm_common_tables.h"

/** @addtogroup STM32F4xx_LowPower_Application
  * @{
  */

/** @addtogroup I2C_LowPower_DataExchange
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC_SAMPLE_TRANSFER 1024
#define I2C_SAMPLE_TRANSFER ADC_SAMPLE_TRANSFER *2 
#define FFT_INVERSE_FLAG  ((uint8_t)0) 
#define FFT_Normal_OUTPUT_FLAG  ((uint8_t)1)
#define LOWER 0 /* lower limit */
#define UPPER ADC_SAMPLE_TRANSFER /* upper limit */

/********** PLL Parameters **********/
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      8
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

#if defined (STM32F40_41xxx)
#define PLL_N      336
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
#endif /* STM32F40_41xxx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define PLL_N      360
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xx)
#define PLL_N      336
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      4
#endif /* STM32F401xx */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_Clocks;
__IO uint16_t aRxBuffer[ADC_SAMPLE_TRANSFER] ;
__IO uint16_t ADCConvertedValue[ADC_SAMPLE_TRANSFER];
__IO uint32_t TimeOut = 0x0;
__IO uint8_t DMA_TC = 0x0;

/* Private function prototypes -----------------------------------------------*/
static void I2C_Config(void);
static void SysTick_Configuration(void);

static void TimeOut_UserCallback(void);
static void GPIO_AnalogConfig(void);
static void StandbyMode(void);
static void SleepMode(void);

#ifdef I2C_MASTER
static void GPIOA5_ConfigIn(void);
static void GPIOA5_ConfigOut(void);
static void StandbyRTCMode(void);
static void FFT_Calc(void);
static void PeriphClock_Disable(void);
static void SysClockSwitch(void);
#endif

#ifdef I2C_SLAVE
static void GPIOA0_ConfigOut(void);
static void ADC_Config(void);
#endif

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 	
/*********************************** Master Code ******************************/
#if defined (I2C_MASTER)
 /* Check that the system was resumed from StandBy mode with WakeUp */ 
  if(PWR_GetFlagStatus(PWR_FLAG_WU) == RESET)
  {
	 /***** Entering in standby mode *****/	
	 #ifdef DEBUG
   DBGMCU_Config(DBGMCU_STANDBY, ENABLE);	
   #endif			
   StandbyMode();
	}
	else
	{			
	/* Clear WakeUp Flag */
  PWR_ClearFlag(PWR_FLAG_WU);
		
	/* Configure All GPIO as analog input */
  GPIO_AnalogConfig();		
		
	/* Toggle PA5 port bit */
	GPIOA5_ConfigOut();	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
		
	/* Waiting until Slave is configured */
	GPIOA5_ConfigIn();
	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) != Bit_SET)
	{}

/* Master Receiver -----------------------------------------------------------*/ 
		
  /* Configure I2C peripheral */		
	I2C_Config();
		
	/* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(I2Cx, ENABLE);
		
  /* Initialize I2C peripheral */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
  I2C_InitStructure.I2C_OwnAddress1 = MASTER_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2Cx, &I2C_InitStructure);
	
	/* I2C ENABLE */
  I2C_Cmd(I2Cx, ENABLE);
		
  /* Send I2Cx START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);
	
	/* SysTick configuration */
	SysTick_Configuration();
	
  /* I2C_7BITS_ADDRESS */	      
  /* Test on I2Cx EV5 and clear it or time out*/
  TimeOut = USER_TIMEOUT;
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))&&(TimeOut != 0x00))
  {}
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  /* Send I2Cx slave Address for write */
  I2C_Send7bitAddress(I2Cx, SLAVE_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on I2Cx EV6 and clear it or time out */
  TimeOut = USER_TIMEOUT;
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&&(TimeOut != 0x00))
  {}
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  /* I2Cx DMA Enable */
  I2C_DMACmd(I2Cx, ENABLE);
  
  /* Enable DMA RX Channel */
  DMA_Cmd(I2Cx_DMA_STREAM_RX, ENABLE);
	
  /* Wait until I2Cx_DMA_STREAM_RX enabled or time out */
  TimeOut = USER_TIMEOUT;
  while ((DMA_GetCmdStatus(I2Cx_DMA_STREAM_RX)!= ENABLE)&&(TimeOut != 0x00))
  {}  
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
		
	/***** Entering in sleep mode ****/
  #ifdef DEBUG
  DBGMCU_Config(DBGMCU_SLEEP, ENABLE);	
  #endif
	SleepMode();  
		
  /* Transfer complete or time out */
  TimeOut = USER_TIMEOUT;
	while ((DMA_TC==0x00)&&(TimeOut != 0x00))	
  {
	  SleepMode();
	}
	DMA_TC=0;	
		
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  /* Send I2Cx STOP Condition */
  I2C_GenerateSTOP(I2Cx, ENABLE);

  /* Disable DMA RX Channel */
  DMA_Cmd(I2Cx_DMA_STREAM_RX, DISABLE);
  
  /* Wait until I2Cx_DMA_STREAM_RX disabled or time out */
  TimeOut = USER_TIMEOUT;
  while ((DMA_GetCmdStatus(I2Cx_DMA_STREAM_RX)!= DISABLE)&&(TimeOut != 0x00))
  {}  
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(I2Cx,DISABLE);
  
  /* Clear any pending flag on Rx Stream  */
  DMA_ClearFlag(I2Cx_DMA_STREAM_RX, I2Cx_RX_DMA_TCFLAG | I2Cx_RX_DMA_FEIFLAG | \
                I2Cx_RX_DMA_DMEIFLAG | I2Cx_RX_DMA_TEIFLAG | I2Cx_RX_DMA_HTIFLAG);
	
	PeriphClock_Disable();
	
	/* System clock switches to maximum frequency */
  SysClockSwitch();
  	
	/* FFT Calculation */
	FFT_Calc();

  /***** Entering in Standby RTC Mode *****/
	#ifdef DEBUG
  DBGMCU_Config(DBGMCU_STANDBY, ENABLE);	
  #endif
	StandbyRTCMode();
  }
 
#endif /* I2C_MASTER_SLAVE */
	
/********************************** Slave Code **********************************/
#if defined (I2C_SLAVE)
	
  /* Check that the system was resumed from StandBy mode with WakeUp */ 
  if(PWR_GetFlagStatus(PWR_FLAG_WU) == RESET)
  {
	 #ifdef DEBUG
   DBGMCU_Config(DBGMCU_STANDBY, ENABLE);	
   #endif			
   StandbyMode();
	}
	else
	{	
	/* Clear WakeUp Flag */
  PWR_ClearFlag(PWR_FLAG_WU);	
		
  /* Configure All GPIO as analog input */
  GPIO_AnalogConfig();	
		
  /* ADC1 Channel Vbat configuration */
  ADC_Config();
	
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC1);
		
	/***** Entering in sleep mode *****/
  #ifdef DEBUG
  DBGMCU_Config(DBGMCU_SLEEP, ENABLE);	
  #endif
	SleepMode();  
	
	/* Disable ADC1 and DMA2 */	
  RCC_APB2PeriphClockCmd(ADCx_CLK, DISABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, DISABLE);

  /* Configure I2C peripheral */	
  I2C_Config();
		
  /* Initialize I2C peripheral */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
  I2C_InitStructure.I2C_OwnAddress1 = SLAVE_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  
  I2C_Init(I2Cx, &I2C_InitStructure);
	
  /* I2C ENABLE */
  I2C_Cmd(I2Cx, ENABLE);

	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != Bit_RESET)
	{}
	
  PWR_WakeUpPinCmd(DISABLE);
	
  /* Toggle PA0 port bit */	
  GPIOA0_ConfigOut();	
  GPIO_SetBits(GPIOA, GPIO_Pin_0);
		
	/* SysTick configuration */
	SysTick_Configuration();
		
/* Slave Transmitter ---------------------------------------------------------*/   
  
  /* Test on I2C EV1 and clear it or time out*/
	TimeOut = USER_TIMEOUT;
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED)&&(TimeOut != 0x00))
  {}
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }		
  /* I2Cx DMA Enable */
  I2C_DMACmd(I2Cx, ENABLE); 
  
  /* Enable DMA TX Channel */
  DMA_Cmd(I2Cx_DMA_STREAM_TX, ENABLE);
		
  /* Wait until I2Cx_DMA_STREAM_TX enabled or time out */
  TimeOut = USER_TIMEOUT;
  while ((DMA_GetCmdStatus(I2Cx_DMA_STREAM_TX)!= ENABLE)&&(TimeOut != 0x00))
  {}  
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
	
	/***** Entering in sleep mode *****/
  #ifdef DEBUG
  DBGMCU_Config(DBGMCU_SLEEP, ENABLE);	
  #endif
	SleepMode();  
  
  /* Transfer complete or time out */
  TimeOut = USER_TIMEOUT;
	while ((DMA_TC==0x00)&&(TimeOut != 0x00))	
  {
		SleepMode(); 
	}
	DMA_TC=0;	
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
	
  /* Test on I2Cx EV3-2 and clear it or time out */
  TimeOut = USER_TIMEOUT;
  while ((!I2C_CheckEvent(I2Cx, I2C_EVENT_SLAVE_ACK_FAILURE))&&(TimeOut != 0x00))
  {}  
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  
  /* Disable DMA TX Channel */
  DMA_Cmd(I2Cx_DMA_STREAM_TX, DISABLE);
  
  /* Wait until I2Cx_DMA_STREAM_TX disabled or time out */
  TimeOut = USER_TIMEOUT;
  while ((DMA_GetCmdStatus(I2Cx_DMA_STREAM_TX)!= DISABLE)&&(TimeOut != 0x00))
  {}  
  if(TimeOut == 0)
  {
    TimeOut_UserCallback();
  }
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(I2Cx,DISABLE);
  
  /* Clear any pending flag on Tx Stream */
  DMA_ClearFlag(I2Cx_DMA_STREAM_TX, I2Cx_TX_DMA_TCFLAG | I2Cx_TX_DMA_FEIFLAG | \
                I2Cx_TX_DMA_DMEIFLAG | I2Cx_TX_DMA_TEIFLAG | I2Cx_TX_DMA_HTIFLAG);
	
	/***** Entering in standby mode *****/
	 #ifdef DEBUG
   DBGMCU_Config(DBGMCU_STANDBY, ENABLE);	
   #endif			
   StandbyMode();	
	}	
#endif /* I2C_SLAVE */
  while(1)
  {}
}

#if defined (I2C_SLAVE)
/**
  * @brief  ADC1 Channel Vbat configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) DMA2_Stream0 channel 0 configuration
            3) Configure ADC1 Channel18 (VBAT) 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  NVIC_InitTypeDef      NVIC_InitStructure;
  
  /* Enable peripheral clocks *************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);

  /* DMA2_Stream0 channel0 configuration **************************************/
  DMA_DeInit(DMA2_Stream0);
  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = ADC_SAMPLE_TRANSFER;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);

	/* Enable DMA2_Stream0 Transfer complete interrupt **************************/
  DMA_ITConfig(DMA_STREAMx, DMA_IT_TC, ENABLE);
    
  /* Enable DMA2_Stream0 IRQn Interrupt ***************************************/
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled ;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADCx, &ADC_InitStructure);

  /* Enable DMA2_Stream0 */
  DMA_Cmd(DMA_STREAMx, ENABLE);
	
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADCx, ENABLE);
  
  /* ADC1 regular channel18 (VBAT) configuration ******************************/
  ADC_RegularChannelConfig(ADCx, ADC_Channel_Vbat, 1, ADC_SampleTime_15Cycles);
	
  /* Enables the EOC on each regular channel conversion ***********************/
  ADC_EOCOnEachRegularChannelCmd(ADCx, ENABLE);
	
  /* Enable VBAT channel */
  ADC_VBATCmd(ENABLE); 

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADCx, ENABLE);
}

/*
  * @brief  Configures GPIO PA0 output pin
  * @param  None
  * @retval None
  */
static void GPIOA0_ConfigOut(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure PA5 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif /* I2C_SLAVE */

/**
  * @brief  Enables the I2C Clock and configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void I2C_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
    
  /* RCC Configuration */
  /* I2C Peripheral clock enable */
  RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
  
  /* SDA GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, ENABLE);
  
  /* SCL GPIO clock enable */
  RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, ENABLE);
  
	/* Deinitialize the I2Cx peripheral registers to their default reset values */
  I2C_DeInit(I2Cx);
  
  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);
	
	/* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
  
  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);
  
  /* GPIO Configuration */
  /* Configure I2C SCL pin */
  GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure I2C SDA pin */
  GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	/* SDA GPIO clock disable */
  RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, DISABLE);
  
  /* SCL GPIO clock disable */
  RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, DISABLE);
	  
  /* DMA Configuration */
  /* Clear any pending flag on Tx Stream  */
  DMA_ClearFlag(I2Cx_DMA_STREAM_TX, I2Cx_TX_DMA_TCFLAG | I2Cx_TX_DMA_FEIFLAG | \
                I2Cx_TX_DMA_DMEIFLAG | I2Cx_TX_DMA_TEIFLAG | I2Cx_TX_DMA_HTIFLAG);
                                       
  /* Clear any pending flag on Rx Stream  */
  DMA_ClearFlag(I2Cx_DMA_STREAM_RX, I2Cx_RX_DMA_TCFLAG | I2Cx_RX_DMA_FEIFLAG | \
                I2Cx_RX_DMA_DMEIFLAG | I2Cx_RX_DMA_TEIFLAG | I2Cx_RX_DMA_HTIFLAG);
  
  /* Disable the I2C Tx DMA stream */
  DMA_Cmd(I2Cx_DMA_STREAM_TX, DISABLE);
  /* Configure the DMA stream for the I2C peripheral TX direction */
  DMA_DeInit(I2Cx_DMA_STREAM_TX);
  
  /* Disable the I2C Rx DMA stream */
  DMA_Cmd(I2Cx_DMA_STREAM_RX, DISABLE);
  /* Configure the DMA stream for the I2C peripheral RX direction */
  DMA_DeInit(I2Cx_DMA_STREAM_RX);
  
  /* Initialize the DMA_Channel member */
  DMA_InitStructure.DMA_Channel = I2Cx_DMA_CHANNEL;
  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStructure.DMA_PeripheralBaseAddr = I2Cx_DR_ADDRESS;
  /* Initialize the DMA_PeripheralInc member */         
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* Initialize the DMA_MemoryInc member */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  /* Initialize the DMA_Priority member */
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  /* Initialize the DMA_FIFOMode member */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  /* Initialize the DMA_FIFOThreshold member */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  /* Initialize the DMA_MemoryBurst member */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  /* Initialize the DMA_PeripheralBurst member */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  /* Init DMA for Reception */
  /* Initialize the DMA_DIR member */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  /* Initialize the DMA_Memory0BaseAddr member */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aRxBuffer;
  /* Initialize the DMA_BufferSize member */
  DMA_InitStructure.DMA_BufferSize = I2C_SAMPLE_TRANSFER;
  DMA_DeInit(I2Cx_DMA_STREAM_RX);
  DMA_Init(I2Cx_DMA_STREAM_RX, &DMA_InitStructure);
	 
	/* Enable DMA1_Stream0 Transfer complete interrupt */
  DMA_ITConfig(I2Cx_DMA_STREAM_RX, DMA_IT_TC, ENABLE);
    
  /* Enable DMA1_Stream0 IRQn Interrupt ***************************************/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Init DMA for Transmission */
  /* Initialize the DMA_DIR member */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  /* Initialize the DMA_Memory0BaseAddr member */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValue;
  /* Initialize the DMA_BufferSize member */
  DMA_InitStructure.DMA_BufferSize = I2C_SAMPLE_TRANSFER;
  DMA_DeInit(I2Cx_DMA_STREAM_TX);
  DMA_Init(I2Cx_DMA_STREAM_TX, &DMA_InitStructure);
	 
	/* Enable DMA1_Stream6 Transfer complete interrupt */
  DMA_ITConfig(I2Cx_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
    
  /* Enable DMA1_Stream6 IRQn Interrupt ***************************************/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 200 ms.
  * @param  None
  * @retval None
  */
static void SysTick_Configuration(void)
{
  /* SysTick end of count event each 200ms */
	RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 5);
  
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

#if defined (I2C_MASTER)
/*
  * @brief  Configures GPIO PA5 output pin
  * @param  None
  * @retval None
  */
static void GPIOA5_ConfigOut(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure PA5 pin as output */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
  * @brief  Configures GPIO PA5 input pin
  * @param  None
  * @retval None
  */
static void GPIOA5_ConfigIn(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure PA5 pin as input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  This function configures the system to enter Standby mode with RTC 
  *         clocked by LSI 
  * @param  None
  * @retval None
  */
static void StandbyRTCMode(void)
{
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* The RTC Clock may varies due to LSI frequency dispersion */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div2, Wakeup Time Base: ~200ms
     RTC Clock Source LSI ~32KHz 

     Wakeup Time Base = (2 / (LSI)) * WakeUpCounter
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);
  RTC_SetWakeUpCounter(3200);
 	
	/* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);
	
	/* Disable WKUP pin */
  PWR_WakeUpPinCmd(DISABLE);
	
	/* Clear Wakeup flag*/ 
  PWR_ClearFlag(PWR_FLAG_WU);
	
  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE); 

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);

  /* Clear WakeUp (WUTF) pending flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);
    
  /* Request to enter STANDBY mode (Wake Up flag is cleared in 
     PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  This function is use to calculate FFT
  * @param  None
  * @retval None
  */
static void FFT_Calc(void)
{
uint16_t Index = LOWER;
uint16_t bIndex = LOWER;
	
static q15_t Buf[I2C_SAMPLE_TRANSFER];	
arm_cfft_radix4_instance_q15  FFT_Q15_struct;

for(bIndex = LOWER	; bIndex < UPPER; bIndex++)
	{ 
		Buf[Index] = aRxBuffer[bIndex];
		Buf[Index+1] = 0;
		Index = Index+2;
	}

arm_cfft_radix4_init_q15(&FFT_Q15_struct, ADC_SAMPLE_TRANSFER, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);	
arm_cfft_radix4_q15(&FFT_Q15_struct, Buf);	
}

/**
  * @brief  Disable non used peripherals
  * @param  None
  * @retval None
  */
static void PeriphClock_Disable(void)
{  
	/* DISABLE GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);
	
  /* DISABLE SCL and SDA GPIO clocks  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE);
  
  /* DISABLE the DMA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, DISABLE);
}	

/**
  * @brief  Switch to maximum clock frequency device
  * @param  None
  * @retval None
  */
static void SysClockSwitch(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
		
    #if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    PWR->CR |= PWR_CR_VOS;
    #endif /* STM32F427_437x || STM32F429_439xx */

    #if defined (STM32F40_41xxx)
    PWR->CR |= PWR_CR_VOS_0;
    #endif /* STM32F40_41xxx */

    #if defined (STM32F401xx)
    PWR->CR |= PWR_CR_VOS_1;
    #endif /* STM32F401xx */		
   
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
    {
    }
    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
    {
    }      
    /* Configure Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif /* STM32F427_437x || STM32F429_439xx  */

#if defined (STM32F40_41xxx)     
    /* Configure Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif /* STM32F40_41xxx  */

#if defined (STM32F401xx)
    /* Configure Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;
#endif /* STM32F401xx */

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}
#endif /* I2C_MASTER */

/*
  * @brief  Configure all GPIO as analog to reduce current consumption on non used IOs
  * @param  None
  * @retval None
  */
void GPIO_AnalogConfig (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable GPIOs clock */

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |  
												 RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | 
												 RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH , ENABLE);

#ifndef STM32F401xx
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | 
												 RCC_AHB1Periph_GPIOI, ENABLE);

#ifdef STM32F429_439xx
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, ENABLE);

#endif	/* STM32F429_439xx */
#endif	/* not STM32F401xx*/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
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
#endif	/* STM32F429_439xx */
#endif	/* STM32F429_439xx || STM32F40_41xxx */

#ifndef DEBUG
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
#endif /* not DEBUG */

  /* Disable GPIOs clock */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                          RCC_AHB1Periph_GPIOC  | RCC_AHB1Periph_GPIOD | \
                          RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOH , DISABLE);

#ifndef STM32F401xx
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | \
                         RCC_AHB1Periph_GPIOI, DISABLE);

#ifdef STM32F429_439xx
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK, DISABLE);
#endif	/* STM32F429_439xx */
#endif	/* not STM32F401xx */
}

/**
  * @brief  Configures the system to enter Standby mode
  * @param  None
  * @retval None
  */

static void StandbyMode(void)
{
	/* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* Clear standby flag */
  PWR_ClearFlag(PWR_FLAG_SB);
	
	/* Disable of RTC wakeup source */
	PWR_BackupAccessCmd(ENABLE);	
	RCC_RTCCLKCmd(DISABLE);	
	
	/* Clear RTC Wakeup flag */ 
  RTC_ClearFlag(RTC_FLAG_WUTF);
	
	/* Clear Wakeup flag */ 
  PWR_ClearFlag(PWR_FLAG_WU);
	
	/* Enable WKUP pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Request to enter STANDBY mode (Wake Up flag is cleared in 
     PWR_EnterSTANDBYMode function) */
   PWR_EnterSTANDBYMode();
}

/**
  * @brief  This function configures the system to enter Sleep mode
  * @param  None
  * @retval None
  */
static void SleepMode(void)
{
	/* Request to enter SLEEP mode */
  __WFI();
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
  while (1)
  {   
  }
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

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
