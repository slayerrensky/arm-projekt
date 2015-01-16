#include "stm32f4xx_usart.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include <stdlib.h>
#include "string.h"

#include "xbee.h"
#include "stepper.h"
#include "static_commandos.h"
#include "ampermeter.h"
#include "adc.h"
#include "fassade.h"
#include "display.h"
#include "terminal.h"
#include "menu.h"

Xbee *XbeeInstance;
char bufferX[256];

Xbee::Xbee(void){
	Xbee(256, XBEE_TYPE_NONE);
}

Xbee::Xbee(int type){
	Xbee(256, type);
}

Xbee::Xbee(int buffersize, int type){

	this->bufferSize = buffersize;
	this->xbeeType = type;
	buffer_init();
	SendFirst = 1;
	this->OutputString = (char*) malloc(sizeof(char)*this->bufferSize);
	XbeeInstance = this;
	this->InitDMA();
	this->Init();

	KommandoBuffer = (char*) malloc(sizeof(char)*KOMMANDO_BUFFER);
	currentKommandoChar = 0;
}

void Xbee::buffer_init(void)
{
	this->read = 0;
	this->write = 0;
	this->buffer = (char*) malloc(sizeof(char)*this->bufferSize);
}

int Xbee::BufferIn(char byte)
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

int Xbee::BufferOut(char *pByte)
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
char* Xbee::ReadBuffer(void)
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
int Xbee::ReadBuffer(char *p)
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
void Xbee::InitDMA()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	//DMA_DeInit(DMA1_Stream5);
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
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

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USART2 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*
 * DMA konfigurieren mit Startadresse der Daten und länge des Strings
 * DMA sendet dann die Daten via Uart
 */
void Xbee::SendViaDma(char *startBuf, int sizeofBytes)
{
	if (XbeeInstance->SendFirst)
	{
		XbeeInstance->SendFirst = 0;
	}
	else
	{
		int a = DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET;
		while (a)
		{
			a = DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET;
		}
		memcpy(XbeeInstance->writeBuffer, startBuf, sizeofBytes);
	}

	DMA_DeInit(DMA1_Stream5);
	USART_ClearFlag(USART2, USART_FLAG_TC);

	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)startBuf;
	DMA_InitStruct.DMA_BufferSize = sizeofBytes;

	DMA_Init(DMA1_Stream5, &DMA_InitStruct);

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

	DMA_Cmd(DMA1_Stream5, ENABLE);

}

/*
 * Uart 3 initialisieren
 */
void Xbee::Init(void)
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
  GPIO_InitTypeDef GPIO_InitStructure2;
  USART_InitTypeDef USART_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure2);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(USART2, &USART_InitStructure);

  /* Enable USART */
  USART_Cmd(USART2, ENABLE);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
/*
 * Ein einzelnes Zeichen senden
 */
void Xbee::PutChar(uint16_t char2send)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, char2send);
}

/*
 * Einen kompletten String senden
 */
void Xbee::SendTransmission( char version, char receiver, char commando, char packetnumber, char *daten , char datalength )
{
	int i;
	char crc = 0x00;
	PutChar((uint16_t) 0x01);
	PutChar((uint16_t) version);
	crc += version;
	char lenght = 5 + datalength;
	PutChar((uint16_t) lenght); //Längenbyte Version bis CRC
	crc += lenght;
	PutChar((uint16_t) receiver);
	crc += receiver;
	PutChar((uint16_t) commando);
	crc += commando;
	PutChar((uint16_t) packetnumber);
	crc += packetnumber;
	for (i=0;i<datalength;i++)
	{
		PutChar((uint16_t) *(daten + i));
		crc += *(daten + i) ;
	}
	PutChar((uint16_t) crc); //Längenbyte Version bis CRC
	PutChar((uint16_t) 0x04);
}

void Xbee::SendMessage(char *massage){
	SendViaDma(massage, strlen(massage));
}

int Xbee::IsCommandoAvalible(){
	//int oldcommando = currentKommandoChar;
	int n; //Anzahl eingelesenener Zeichen
	int i; //Forschleivenzählvariable
	n = ReadBuffer(KommandoBuffer + currentKommandoChar);
	currentKommandoChar += n;
	for (i = 0; i < bufferSize;i++)
	{
		if (KommandoBuffer[i] == 0x04) // EOT (End of Transmission)
		{	//Befehl vollständig
			KommandoTerminator = i;
			return 1;
		}
	}
	return 0;
}

void Xbee::ProzessCommando(){
		int EOTByte = 0;
		int SOHByte = 0;
		int tl; //Transmission lenght
		int i = 0;
		for(i=0;i<KommandoTerminator;i++)
		{
			if (KommandoBuffer[i] == 0x01)
			{
				SOHByte = i;
				break;
			}

		}
		if (KommandoBuffer[KommandoTerminator] == 0x04)
		{
			if( KommandoBuffer[KommandoTerminator + 1] == 0x04)
				KommandoTerminator++;
		}

		tl = KommandoTerminator-SOHByte+1;
		char* cmd = (char*) malloc(sizeof(char) * tl ); //Speicher resavieren für das aktuelle kommando
		memcpy(cmd, KommandoBuffer,tl); // Transmission sichern
		memcpy(KommandoBuffer, KommandoBuffer + KommandoTerminator + 1 ,bufferSize - tl); //Buffer nach vorne verschieben
		currentKommandoChar -= tl; // Anfangszeiger setzen
		CommandoProzess(cmd);
		free(cmd);
		KommandoTerminator = 0;
}

/*
 * Verarbeiten der Kommandos die vom Xbee kommen.
 */
void Xbee::CommandoProzess(char *transmission){
	int i=0;
	int startbyte = 0;
	while (transmission[startbyte] != 0x01)
	{
		startbyte++;
	}
	int version = transmission[startbyte + 1];
	int laenge = transmission[startbyte + 2];
	char crc = transmission[startbyte + laenge+1];
	int commando;
	int paketnummer;
	int dataStart;
	int dataEnd;
	if(transmission[startbyte+laenge+2] == 0x04 )
	{
		char crc_sum = 0;
		int i;
		for (i=startbyte+1;i<laenge+1;i++) // CRC von byte 1 bis zum CRC byte (ohne CRC)
		{
			crc_sum += transmission[i];
		}
		if (crc_sum == crc )
		{
			if (version == XBEE_PROTOKOLL_VERSION)
			{
				commando = transmission[startbyte + 4];
				paketnummer = transmission[startbyte + 5];
				dataStart = startbyte + 6;
				dataEnd = startbyte + laenge;
			}
			else
			{
				TerminalInstance->SendMessage("\n\rDebug: Unknown version number->\n\r");
				return;
			}
		}
		else
		{
			TerminalInstance->SendMessage("\n\rDebug: CRC is wrong\n\r");
			TerminalInstance->SendViaDma(transmission,laenge + 2);
			return;
		}
	}
	else
	{
		TerminalInstance->SendMessage("\n\rDebug: End byte not right->\n\r");
		TerminalInstance->SendViaDma(transmission,laenge + 2);
		return;
	}

	if (commando == 0x10)
	{
		DisplayInstance->SendByte(transmission + dataStart, dataEnd - dataStart+1, DISPLAY_SOURCE_LOCAL);
	}
	else if (commando == XBEE_COM_SET_SOLLWERT_VALUE)
	{
		for (i=0;i<dataEnd - dataStart+1;i++)
		{
			bufferX[i] = *(transmission + dataStart + i);
		}
		bufferX[dataEnd - dataStart+2] = 0x00;
		FassadeInstance->SetSolltemp(atoff(bufferX));
	}
	else if (commando == XBEE_COM_INFO_SOLLWERT_VALUE)
	{
		for (i=0;i<dataEnd - dataStart+1;i++)
		{
			bufferX[i] = *(transmission + dataStart + i);
		}
		bufferX[dataEnd - dataStart+2] = 0x00;
		MenuInstance->SetSollwert(atoff(bufferX));
	}
	else if (commando == XBEE_COM_INFO_DEFAULT_SCREEN)
	{
		menuShow = false;
		FassadeInstance->UpdateDisplayValues();
	}
	else if (commando == XBEE_COM_GETVALUE)
	{
		switch(*(transmission + dataStart ))
		{
			case XBEE_COM_INFO_SOLLWERT_VALUE:
			{
				sprintf(bufferX,"%f",FassadeInstance->GetSolltemp());
					XbeeInstance->SendTransmission(XBEE_PROTOKOLL_VERSION, XBEE_TYPE_REMOUTE,
							XBEE_COM_INFO_SOLLWERT_VALUE,(char) 0x00,  bufferX ,(char)strlen(bufferX));
			}
			default:;
		}
	}

}

/*
 * Singelton Zeiger schreiben
 */
void Xbee::EnableSingelton(void)
{
	XbeeInstance = this;
}

/////////////////////////////////////////
// Begin Interrupt Funktionen
//////////////////////////////////////////

/*
 * DMA1- Interrupt
 */
extern "C" { void DMA1_Stream5_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
  }
}}

/*
 *  UART2-Interrupt
 */
extern "C" { void USART2_IRQHandler(void) {
	char wert;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		// wenn ein Byte im Empfangspuffer steht
		wert=USART_ReceiveData(USART2);
		// Byte speichern
		XbeeInstance->txin++;
		XbeeInstance->BufferIn(wert);
	}
}}
