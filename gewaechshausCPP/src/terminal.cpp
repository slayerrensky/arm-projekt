#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include <stdlib.h>
#include "string.h"

#include "terminal.h"
#include "stepper.h"
#include "static_commandos.h"
#include "ampermeter.h"
#include "adc.h"
#include "fassade.h"
#include "display.h"
#include "defines.h"

char writeBuffer[256];

Terminal *TerminalInstance;

Terminal::Terminal(void){
	Terminal(128);
}

Terminal::Terminal(int buffersize){

	this->bufferSize = buffersize;
	buffer_init();
	SendFirst = 1;
	this->OutputString = (char*) malloc(sizeof(char)*this->bufferSize);
	TerminalInstance = this;
	this->usart3InitDMA();
	this->usart3Init();

	KommandoBuffer = (char*) malloc(sizeof(char)*KOMMANDO_BUFFER);
	currentKommandoChar = 0;

}

void Terminal::buffer_init(void)
{
	this->read = 0;
	this->write = 0;
	this->buffer = (char*) malloc(sizeof(char)*this->bufferSize);
}

int Terminal::BufferIn(char byte)
{
  //if (this->buffer.this->write >= BUFFER_SIZE)
  //  this->buffer.this->write = 0; // erhöht sicherheit

  if (this->write + 1 == this->read || (this->read == 0 && this->write + 1 == bufferSize))
    return FAIL; // voll

  this->buffer[this->write] = byte;

  this->write = this->write + 1;
  if (this->write >= bufferSize)
	  this->write = 0;
  return SUCCESS;

}

int Terminal::BufferOut(char *pByte)
{
  if (this->read == this->write)
    return -1;
  *pByte = this->buffer[this->read];

  this->read = this->read + 1;
  if (this->read >= bufferSize)
	  this->read = 0;
  return 0;
}

/*
 * Ließt den Input Buffer aus
 * Note: Bitte den Speicher freigeben wenn der String nicht mehr benötigt wird.
 * Return: Zeiger auf *char
 */
char* Terminal::ReadBuffer(void)
{
	int count = 0;
	char *out;
	while(BufferOut(OutputString + count) == 0)
	{
		count++;
	}
	if (count <= 0 )
		return NULL;
	OutputString[count] = 0;
	out = (char*) malloc(sizeof(char) * count+1);
	strcpy(out, OutputString);
	return out;
}

/*
 * Ließt den Input Buffer aus
 * Note: Bitte den Speicher freigeben wenn der String nicht mehr benötigt wird.
 * Return: Zeiger auf *char
 */
int Terminal::ReadBuffer(char *p)
{
	int count = 0;

	while(BufferOut(p + count) == 0)
	{
		count++;
	}
	if (count <= 0 )
		return 0;
	p[count] = 0;
		return count;
}

/*
 * Init DMA Initialisieren
 * DMA wird so initialisiert das er Daten über den Uart senden kann.
 */
void Terminal::usart3InitDMA()
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * DMA konfigurieren mit Startadresse der Daten und länge des Strings
 * DMA sendet dann die Daten via Uart
 */
void Terminal::SendViaDma(char *startBuf, int sizeofBytes)
{
	if (TerminalInstance->SendFirst)
	{
		TerminalInstance->SendFirst = 0;
	}
	else
	{
		int a = DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET;
		while (a)
		{
			a = DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == RESET;
		}
		memcpy(writeBuffer, startBuf, sizeofBytes);
	}

	DMA_DeInit(DMA1_Stream3);
	USART_ClearFlag(USART3, USART_FLAG_TC);

	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)writeBuffer;
	DMA_InitStruct.DMA_BufferSize = sizeofBytes;

	DMA_Init(DMA1_Stream3, &DMA_InitStruct);

	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);

	DMA_Cmd(DMA1_Stream3, ENABLE);

}

/*
 * Uart 3 initialisieren
 */
void Terminal::usart3Init(void)
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

  USART_InitStructure.USART_BaudRate = 9600;
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
 * Ein einzelnes Zeichen senden
 */
void Terminal::uartPutChar(uint16_t char2send)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, char2send);
}

/*
 * Einen kompletten String senden
 */
void Terminal::uartSendString( char *ptr )
{
  // sende String (So lange bis \0 byte kommt)
  while (*ptr != 0) {
	  uartPutChar((uint16_t) *ptr);
	  ptr++;
  }
}

void Terminal::SendMessage(char *massage){
	SendViaDma(massage, strlen(massage));
}
int Terminal::IsCommandoAvalible(){
	int oldcommando = currentKommandoChar;
	int n; //Anzahl eingelesenener Zeichen
	int i; //Forschleivenzählvariable
	n = ReadBuffer(KommandoBuffer + currentKommandoChar);
	currentKommandoChar += n;
	for (i = oldcommando; i < currentKommandoChar;i++)
	{
		if (KommandoBuffer[i] == '\r')
		{	//Befehl vollständig
			KommandoTerminator = i;
			return 1;
		}
		else
		{
			uartPutChar(KommandoBuffer[i]);
		}
	}

	return 0;
}

void Terminal::ProzessCommando(){
	char* cmd = (char*) malloc(sizeof(char) * KommandoTerminator + 1);
	KommandoBuffer[KommandoTerminator] = '\0';
	strcpy(cmd, KommandoBuffer);
	strcpy(KommandoBuffer, KommandoBuffer + KommandoTerminator+1);
	currentKommandoChar -= KommandoTerminator+1;
	KommandoTerminator = 0;
	CommandoProzess(cmd);
}

/*
 * Verarbeiten der Kommandos die vom Terminal kommen.
 */
void Terminal::CommandoProzess(char *commando){
	char delimiter[] = " ";
	char *ptr;

	ptr = strtok(commando, delimiter);

	if (ptr != NULL)
	{
		if (strcmp(ptr,"left")==0)
		{
			ptr = strtok(NULL, delimiter);
			if (strcmp(ptr,"-n")==0)
			{
				ptr = strtok(NULL, delimiter);
				int n = atoi(ptr);
				StepperInstance->Left(n,20);
			}
			else
			uartSendString((char *)"\r\nUsage: left -n <int>");

		}
		else if (strcmp(commando,"right")==0)
		{
			ptr = strtok(NULL, delimiter);
			if (strcmp(ptr,"-n")==0)
			{
				ptr = strtok(NULL, delimiter);
				int n = atoi(ptr);
				StepperInstance->Right(n,20);
			}
			else
			uartSendString((char *)"\r\nUsage: right -n <int>");

		}
		else if (strcmp(commando,"current")==0)
		{
			sprintf(tmpBuffer, "\r\nCurrent: %1.3fA",AmpermeterInstance->getCurrent());
			uartSendString(tmpBuffer);
		}
		else if (strcmp(commando,"voltage")==0)
		{
			sprintf(tmpBuffer, "\r\nVoltage: %1.3fV",AnalogDigitalConverterInstance->getConvertedValueAsVoltage(1));
			uartSendString(tmpBuffer);
		}
		else if (strcmp(commando,"temp")==0)
		{
			FassadeInstance->TerminalDisplayTemp();
		}
		else if (strcmp(commando,"window")==0)
		{
			ptr = strtok(NULL, delimiter);
			int n = atoi(ptr);
			if(n >= 0 && n <=100)
			{
				FassadeInstance->Window2Position(n);
			}
			else
			uartSendString((char *)"\r\nUsage: right <int> from 0 to 100");
		}
		else if (strcmp(commando,"display")==0)
		{
			ptr = strtok(NULL, delimiter);
			if (strcmp(ptr,"-l")==0)
			{
				ptr = strtok(NULL, delimiter);
				int n = atoi(ptr);
				if(n >= 0 && n <=29)
				{
#ifdef TYPE_CORE
					DisplayInstance->Backlight(n, DISPLAY_SOURCE_REMOUTE);
#endif
#ifdef TYPE_REMOUTE
					DisplayInstance->Backlight(n, DISPLAY_SOURCE_LOCAL);
#endif


				}
			}
			else
			uartSendString((char *)"\r\nUsage: display -l <int> from 0 to 29");
		}
		else
		uartSendString(MessageHelp);

	}
	uartSendString((char *)"\r\n");
}

/*
 * Singelton Zeiger schreiben
 */
void Terminal::EnableSingelton(void)
{
	TerminalInstance = this;
}

/////////////////////////////////////////
// Begin Interrupt Funktionen
//////////////////////////////////////////

/*
 * DMA1- Interrupt
 */
extern "C" { void DMA1_Stream3_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
  }
}}

/*
 *  UART3-Interrupt
 */
extern "C" { void USART3_IRQHandler(void) {
	char wert;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
		// wenn ein Byte im Empfangspuffer steht
		wert=USART_ReceiveData(USART3);
		// Byte speichern
		TerminalInstance->BufferIn(wert);
	}
}}
