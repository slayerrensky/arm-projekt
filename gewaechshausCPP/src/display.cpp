#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include <stdlib.h>
#include "string.h"

#include "display.h"

Display *DisplayInstance;

Display::Display(void){
	Display(128);
}

Display::Display(int buffersize){

	SendFirst = 1;
	DisplayInstance = this;
	//this->InitDMA();
	this->Init();
}

/*
 * Init DMA Initialisieren
 * DMA wird so initialisiert das er Daten über den Uart senden kann.
 */
void Display::InitDMA()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	//DMA_DeInit(DMA1_Stream3);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //normal
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable; //DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USART3 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * DMA konfigurieren mit Startadresse der Daten und länge des Strings
 * DMA sendet dann die Daten via Uart
 */
void Display::SendViaDma(char *startBuf, int sizeofBytes)
{
	if (DisplayInstance->SendFirst)
	{
		DisplayInstance->SendFirst = 0;
	}
	else
	{
		int a = DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET;
		while (a)
		{
			a = DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET;
		}
	}

	DMA_DeInit(DMA1_Stream3);
	USART_ClearFlag(USART3, USART_FLAG_TC);

	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)startBuf;
	DMA_InitStruct.DMA_BufferSize = sizeofBytes;

	DMA_Init(DMA1_Stream3, &DMA_InitStruct);

	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);

	DMA_Cmd(DMA1_Stream3, ENABLE);

}

/*
 * Uart 4 initialisieren
 */
void Display::Init(void)
{
  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(UART4, &USART_InitStructure);

  /* Enable USART */
  USART_Cmd(UART4, ENABLE);

//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

}
/*
 * Ein einzelnes Zeichen senden
 */
void Display::uartPutChar(uint16_t char2send)
{
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	USART_SendData(UART4, char2send);
}

/*
 * Einen kompletten String senden
 */
void Display::SendString( char *ptr )
{
  // sende String (So lange bis \0 byte kommt)
  while (*ptr != 0) {
	  uartPutChar((uint16_t) *ptr);
	  ptr++;
  }
}

void Display::SendMessage(char *massage){
	SendViaDma(massage, strlen(massage));
}

void Display::SendByte( char *ptr , int lenght)
{
	int i;
	for (i=0;i<lenght;i++)
		uartPutChar((uint16_t) *(ptr + i));
}

/*
 * Singelton Zeiger schreiben
 */
void Display::EnableSingelton(void)
{
	DisplayInstance = this;
}

/*
 * Set Backlight litning stange
 * from 0 to 29.
 */
void Display::Backlight(char value)
{
	if (value >= 0 && value <30)
	{
		DisplayInstance->uartPutChar(0x7c);
		DisplayInstance->uartPutChar(128 + value);
	}
}

/*
 * Send Special character
 */
void Display::SpecialCommand(char value)
{
	DisplayInstance->uartPutChar(254);
	DisplayInstance->uartPutChar(value);
}

/*
 * @brief  Set Cursor Position.
 * @param Line 1 Position 0-19
 * 		  Line 2 Position 64-83
 * 		  Line 3 Position 20-39
 * 		  Line 4 Position 84-103
 */
void Display::SetCursorPosition(char position)
{
	DisplayInstance->uartPutChar(254);
	DisplayInstance->uartPutChar(0x80 + position);
}


/////////////////////////////////////////
// Begin Interrupt Funktionen
//////////////////////////////////////////

///*
// * DMA1- Interrupt
// */
//extern "C" { void DMA1_Stream3_IRQHandler(void)
//{
//  /* Test on DMA Stream Transfer Complete interrupt */
//  if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3))
//  {
//    /* Clear DMA Stream Transfer Complete interrupt pending bit */
//    DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
//  }
//}}

///*
// *  UART3-Interrupt
// */
//extern "C" { void USART3_IRQHandler(void) {
//	char wert;
//
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
//		// wenn ein Byte im Empfangspuffer steht
//		wert=USART_ReceiveData(USART3);
//		// Byte speichern
//		DisplayInstance->BufferIn(wert);
//	}
//}}
