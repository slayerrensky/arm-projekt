/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"


class Xbee
{
public:
	Xbee();
};

void usart3_init(void);
void uartPutChar(uint16_t char2send);
void uartSendString( char *ptr );
