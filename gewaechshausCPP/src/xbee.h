/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#ifndef XBEE_H_
#define XBEE_H__
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#define FAIL -1;
#define SUCSESS 0;
#define KOMMANDO_BUFFER 512

class Xbee {

public:
	Xbee();
	Xbee(int buffersize);
	void Init(void);
	void InitDMA();
	void PutChar(uint16_t char2send);
	void SendString( char *ptr );
	int BufferOut(char *pByte);
	int BufferIn(char byte);
	char* ReadBuffer(void);
	int ReadBuffer(char *p);
	void SendViaDma(char *startBuf, int sizeofBytes);
	void EnableSingelton(void);

	void SendMessage(char *massage);
	int IsCommandoAvalible();
	void ProzessCommando();
	char tmpBuffer[KOMMANDO_BUFFER];


protected:
	char *buffer;
	int bufferSize;
	int read;  // Zeit auf das Feld wo gelesen werden kann
	int write; // Zeigt auf das feld zum beschreiben
	char *OutputString;
	DMA_InitTypeDef DMA_InitStruct;
	void buffer_init(void);
	int SendFirst;

	void CommandoProzess(char *commando);
	char *KommandoBuffer;
	int currentKommandoChar;
	int KommandoTerminator;

};

extern Xbee *XbeeInstance;
#endif
