/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
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
