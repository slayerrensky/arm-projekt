/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#ifndef DISPLAY_H_
#define DISPLAY_H__
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

#define FAIL -1;
#define SUCSESS 0;
#define KOMMANDO_BUFFER 512

class Display {

public:
	Display();
	Display(int buffersize);
	void Init(void);
	void InitDMA();
	void uartPutChar(uint16_t char2send);
	void SendString( char *ptr );

	void SendViaDma(char *startBuf, int sizeofBytes);
	void EnableSingelton(void);
	void Backlight(char value);
	void SpecialCommand(char value);
	void SetCursorPosition(char line, char pos);

	void SendMessage(char *massage);
	void SendByte( char *ptr , int lenght);


protected:
	char *OutputString;
	DMA_InitTypeDef DMA_InitStruct;
	int SendFirst;

};

extern Display *DisplayInstance;

#define DISPLAY_ClearDisplay 0x01
#define DISPLAY_MoveCursorRightOne 0x14
#define DISPLAY_MoveCursorLeftOne 0x10
#define DISPLAY_ScrollRight 0x1c
#define DISPLAY_ScrollLeft 0x18
#define DISPLAY_TrunVisualDisplayOn 0x0C
#define DISPLAY_TrunVisualDisplayOff 0x08
#define DISPLAY_UnderlineCursorOn 0x0E
#define DISPLAY_UnderlineCursorOff 0x0C
#define DISPLAY_BlinkingBoxCoursorOn 0x0D
#define DISPLAY_BlinkingBoxCoursorOff 0x0C





#endif
