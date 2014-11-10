//--------------------------------------------------------------
// File     : main.c
// Datum    : 23.06.2013
// Version  : 1.4
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der USB-CDC-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_usb_cdc.h"

int main(void)
{
  char buf[APP_TX_BUF_SIZE]; // puffer fuer Datenempfang
  USB_CDC_RXSTATUS_t check=RX_EMPTY;

  SystemInit(); // Quarz Einstellungen aktivieren

  // Init vom USB-OTG-Port als CDC-Device
  // (Virtueller-ComPort)
  UB_USB_CDC_Init();

  while(1)
  {
	  // Test ob USB-Verbindung zum PC besteht
	  if(UB_USB_CDC_GetStatus()==USB_CDC_CONNECTED) {
		  // Ceck ob Daten per USB empfangen wurden
		  check=UB_USB_CDC_ReceiveString(buf);
		  if(check==RX_READY) {
			  // wenn Daten empfangen wurden
			  // als Echo wieder zurücksenden
			  // (mit LineFeed+CarriageReturn)
			  UB_USB_CDC_SendString(buf,LFCR);
		  }
	  }
  }
}
