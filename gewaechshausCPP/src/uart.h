/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"

#define FAIL -1;
#define SUCSESS 0;



class Usart {

public:
	Usart();
	Usart(int buffersize);
	void usart3Init(void);
	void usart3InitDMA(char *startBuf, int sizeofBytes);
	void uartPutChar(uint16_t char2send);
	void uartSendString( char *ptr );
	int BufferOut(char *pByte);
	int BufferIn(char byte);

protected:
	char *buffer;
	int bufferSize;
	int read;  // Zeit auf das Feld wo gelesen werden kann
	int write; // Zeigt auf das feld zum beschreiben

	void buffer_init(void);

};

extern Usart *Usart3Instance;

