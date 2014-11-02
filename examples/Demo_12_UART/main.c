//--------------------------------------------------------------
// File     : main.c
// Datum    : 24.02.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der UART-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_uart.h"

int main(void)
{
  char buf[RX_BUF_SIZE]; // puffer fuer Datenempfang
  UART_RXSTATUS_t check;

  SystemInit(); // Quarz Einstellungen aktivieren

  // Init der UARTs
  // Com2 und Com3 mit je 115200 Baud
  UB_Uart_Init();

  // Zwei String per UART senden (mit LineFeed+CarriageReturn)
  UB_Uart_SendString(COM2,"Hier ist UART2 [PA2,PA3]",LFCR);
  UB_Uart_SendString(COM3,"Hier ist UART3 [PD8,PD9]",LFCR);

  while(1)
  {
	  // check ob Daten an UART2 empfangen werden
	  check=UB_Uart_ReceiveString(COM2,buf);
	  if(check==RX_READY) {
		  // wenn Daten empfangen wurden
		  // als Echo wieder zurücksenden
		  UB_Uart_SendString(COM2,buf,LFCR);
	  }
  }
}
