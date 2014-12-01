/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#ifndef UART_H_
#define UART_H__
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#define FAIL -1;
#define SUCSESS 0;



class Usart {

public:
	Usart();
	Usart(int buffersize);
	void usart3Init(void);
	void usart3InitDMA();
	void uartPutChar(uint16_t char2send);
	void uartSendString( char *ptr );
	int BufferOut(char *pByte);
	int BufferIn(char byte);
	char* ReadBuffer(void);
	void SendViaDma(char *startBuf, int sizeofBytes);
	void EnableSingelton(void);

protected:
	char *buffer;
	int bufferSize;
	int read;  // Zeit auf das Feld wo gelesen werden kann
	int write; // Zeigt auf das feld zum beschreiben
	char *OutputString;
	DMA_InitTypeDef DMA_InitStruct;
	void buffer_init(void);
	int SendFirst;

};

extern Usart *Usart3Instance;
#endif
