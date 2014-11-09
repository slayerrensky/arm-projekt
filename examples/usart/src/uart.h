/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"
#define BUFFERSIZE 128

char uart3Buffer[BUFFERSIZE];
uint arrowGetter;
uint arrowSetter;
uint arrowCurrent;

void usart3Init(void);
void uartPutChar(uint16_t char2send);
void uartSendString( char *ptr );
