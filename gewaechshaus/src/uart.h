/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"
#define BUFFERSIZE 128
#define FAIL -1;
#define SUCSESS 0;

	extern char *buffer;
	extern int read;  // Zeit auf das Feld wo gelesen werden kann
	extern int write; // Zeigt auf das feld zum beschreiben

	void usart3Init(void);
	void uartPutChar(uint16_t char2send);
	void uartSendString( char *ptr );
	void SendViaDma(char *startBuf, int sizeofBytes);

	void buffer_init(void);
	int BufferIn(char byte);
	int BufferOut(char *pByte);

