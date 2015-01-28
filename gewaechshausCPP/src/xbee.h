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
#define XBEE_TYPE_NONE (char) 0x00
#define XBEE_TYPE_CORE (char) 0x01
#define XBEE_TYPE_REMOUTE (char) 0x02
#define XBEE_TYPE_NETWORK_MANAGER (char) 0x03
#define XBEE_PROTOKOLL_VERSION (char)0x00

#define XBEE_COM_DISPLAY_TEXT (char)0x10
#define XBEE_COM_ROTARY (char)0x20
#define XBEE_COM_ROTARY_BUTTON (char)0x21
#define XBEE_COM_BUTTON_A (char)0x22
#define XBEE_COM_BUTTON_B (char)0x23
#define XBEE_COM_GETVALUE (char)0x30
#define XBEE_COM_SET_SOLLWERT_VALUE (char)0x31
#define XBEE_COM_VALUE_SOLLWERT (char)0x32
#define XBEE_COM_VALUE_INDOOR (char)0x44
#define XBEE_COM_VALUE_OUTDOOR (char)0x45
#define XBEE_COM_VALUE_WINDOW (char)0x40
#define XBEE_COM_VALUE_CURRENT (char)0x42
#define XBEE_COM_VALUE_VOLTAGE (char)0x43

#define XBEE_COM_INFO_DEFAULT_SCREEN (char)0x60


extern char bufferX[256];

struct PValues {
	float indor;
	float outdor;
	float sollwert;
	int windowP;
	float current;
	float voltage;
};

class Xbee {

public:
	Xbee();
	Xbee(int type);
	Xbee(int buffersize,int type);
	void Init(void);
	void InitDMA();
	void PutChar(uint16_t char2send);
	void SendTransmission( char version, char receiver, char commando, char packetnumber, char *daten , char datalenght);
	int BufferOut(char *pByte);
	int BufferIn(char byte);
	char* ReadBuffer(void);
	int ReadBuffer(char *p);
	void SendViaDma(char *startBuf, int sizeofBytes);
	void EnableSingelton(void);

	void SendMessage(char *massage);
	int IsCommandoAvalible();
	void ProzessCommando();
	void TransmittPValues();
	char tmpBuffer[KOMMANDO_BUFFER];
	int xbeeType;
	char dmaBuffer[256];
	char writeBuffer[256];
	int txin = 0;
	PValues values = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};



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
	unsigned int currentKommandoChar;
	unsigned int KommandoTerminator;

};

extern Xbee *XbeeInstance;
#endif
