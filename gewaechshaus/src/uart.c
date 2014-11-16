#include "uart.h"
#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "misc.h"
#include <stdlib.h>


int read;  // Zeit auf das Feld wo gelesen werden kann
int write; // Zeigt auf das feld zum beschreiben
char *buffer;

int BufferOut(char *pByte)
{
  if (read == write)
    return FAIL;
  *pByte = buffer[read];

  read = read + 1;
  if (read >= BUFFERSIZE)
	  read = 0;
  return SUCCESS;
}


int BufferIn(char byte)
{
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit

  if (write + 1 == read || ( read == 0 && write + 1 == BUFFERSIZE))
    return FAIL; // voll

  buffer[write] = byte;

  write = write + 1;
  if (write >= BUFFERSIZE)
	  write = 0;
  return SUCCESS;

}


/*
 * Uart 3 initialisieren
 */
void usart3Init(void)
{
	read = 0;
	write = 0;
	buffer = (char*) malloc (BUFFERSIZE * sizeof(char));
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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(USART3, &USART_InitStructure);

  /* Enable USART */
  USART_Cmd(USART3, ENABLE);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*
 * DMA konfigurieren mit Startadresse der Daten und länge des Strings
 * DMA sendet dann die Daten via Uart
 */
void SendViaDma(char *startBuf, int sizeofBytes)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStruct;
	DMA_StructInit(&DMA_InitStruct);



	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //normal
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable; //DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	// ob das klappt
	//while (DMA_GetCurrDataCounter(DMA1_Stream3) == 0);

	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)startBuf;
	DMA_InitStruct.DMA_BufferSize = sizeofBytes;

	DMA_Init(DMA1_Stream3, &DMA_InitStruct);

	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);

	DMA_FlowControllerConfig(DMA1_Stream3, DMA_FlowCtrl_Memory);
	DMA_Cmd(DMA1_Stream3, ENABLE);

}

/*
 * Ein einzelnes Zeichen senden
 */
void uartPutChar(uint16_t char2send)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, char2send);
}

/*
 * Einen kompletten String senden
 */
void uartSendString( char *ptr )
{
  // sende String (So lange bis \0 byte kommt)
  while (*ptr != 0) {
	  uartPutChar((uint16_t) *ptr);
	  ptr++;
  }
}


void DMA1_Stream3_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF3))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
  }
}

/*
 *  UART3-Interrupt
 */
void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {

		char wert;
		// wenn ein Byte im Empfangspuffer steht
		wert=USART_ReceiveData(USART3);
		// Byte speichern
		BufferIn(wert);
  }
}
