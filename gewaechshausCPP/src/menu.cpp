#include "menu.h"
#include "stm32f4xx.h"
#include "display.h"
#include "xbee.h"
#include <stdio.h>
#include "string.h"
#include "temp_sensors.h"
#include "defines.h"
#include "fassade.h"

Menu *MenuInstance;


char MainMenuTable[3][20] = {"Hauptmenu",  //NAME
    	  	  	  	  	    "Sensoren anzeigen",
    	  	  	  	  	  	"Sollwert festlegen"};

char MainSollwertTable[1][20] = {"Sollwert Innen"};

char MainSubmitTable[3][20] = {"Uebernehmen?",  //NAME
							  "Ja",
							  "Nein"};

bool menuShow = false;

menu_str MenuMain={MENU_MAIN, 3, (char*)MainMenuTable};
menu_str MenuSollwert={MENU_SOLLWERT, 1, (char*)MainSollwertTable};
menu_str MenuSubmit={MENU_SUBMIT, 3, (char*)MainSubmitTable};

Menu::Menu(void){

	MenuInstance = this;
	this->Init();

}

void Menu::Init(void)
{
	MenuInstance->menu=0;
	MenuInstance->cursor=0;
}

void Menu::PrintMenu(menu_str *menu){

	DisplayInstance->SpecialCommand(DISPLAY_ClearDisplay, DISPLAY_SOURCE_LOCAL);
	MenuInstance->currentMenu=menu;
	int i = 0;
	for(;i<menu->Count;i++)
	{
		DisplayInstance->SetCursorPosition(i,1,DISPLAY_SOURCE_LOCAL);
		DisplayInstance->SendString(menu->menuEntrys+(i*20),DISPLAY_SOURCE_LOCAL);
	}
}

void Menu::MenuUP(void)
{
	MenuInstance->Init();

	DisplayMenu(MENU_MAIN);
}

void Menu::Cursor(int i)
{
	MenuInstance->cursor += (int)i/4;


	switch (MenuInstance->menu)
	{
		case MENU_SOLLWERT:
		{
			MenuInstance->sollwert += (0.25 * int(i/4));
			DisplayInstance->SetCursorPosition(2,0,DISPLAY_SOURCE_LOCAL);
			sprintf(bufferD," Soll: %2.2f",MenuInstance->sollwert);
			DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
		}
		default:
		{
			if (MenuInstance->cursor > MenuInstance->currentMenu->Count-2 )
				MenuInstance->cursor = MenuInstance->currentMenu->Count-2;
			if (MenuInstance->cursor < 0 )
				MenuInstance->cursor = 0;
			DrawCursor();
		}
	}
}

void Menu::Submit()
{
	switch (MenuInstance->menu)
	{
		case MENU_MAIN:
		{
			DisplayMenu(MenuInstance->cursor+1);
			//GET Value Sollwert

			//Print sollwert;
			//DisplayInstance->SetCursorPosition(2,0,DISPLAY_SOURCE_LOCAL);
			//sprintf(bufferD," Soll: %2.2f",sollwert);
			//DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
			break;
		}
		case MENU_SENSOREN:
		{
			break;
		}
		case MENU_SOLLWERT:
		{
			MenuInstance->menuBefore = MenuInstance->menu;
			DisplayMenu(MENU_SUBMIT);
			break;
		}
		case MENU_SUBMIT:
		{
			switch (MenuInstance->menuBefore)
			{
				case MENU_SOLLWERT:
				{
					if (MenuInstance->cursor == 0)
					{

						sprintf(bufferX,"%f",sollwert);

						XbeeInstance->SendTransmission(XBEE_PROTOKOLL_VERSION, XBEE_TYPE_CORE,
								XBEE_COM_SET_SOLLWERT_VALUE,(char) 0x00,  bufferX ,(char)strlen(bufferX));
						DisplayMenu(MENU_MAIN);

					}
					else
					{
						DisplayMenu(menuBefore);
					}
					break;
				}
				default:;

			}
			DisplayMenu(MENU_MAIN);
			break;
		}
		default: break;
	}
}

void Menu::DisplayMenu(int menu)
{
	MenuInstance->cursor = 0;
	switch (menu)
	{
		case MENU_MAIN:
		{
			PrintMenu(&MenuMain);
			DrawCursor();
			break;
		}
		case MENU_SENSOREN:
		{
			menuShow = false;
			FassadeInstance->UpdateDisplayValues();
			//sprintf(bufferX, "%c", XBEE_COM_INFO_DEFAULT_SCREEN);
			//XbeeInstance->SendTransmission(XBEE_PROTOKOLL_VERSION, XBEE_TYPE_REMOUTE,
					//XBEE_COM_INFO_DEFAULT_SCREEN,(char) 0x00,  bufferX ,(char)strlen(bufferX));
			break;
		}
		case MENU_SOLLWERT:
		{
			PrintMenu(&MenuSollwert);
			MenuInstance->sollwert = XbeeInstance->values.sollwert;
			DisplayInstance->SetCursorPosition(2,0,DISPLAY_SOURCE_LOCAL);
			sprintf(bufferD," Soll: %2.2f",MenuInstance->sollwert);
			DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
			break;
		}
		case MENU_SUBMIT:
		{
			PrintMenu(&MenuSubmit);
			DrawCursor();
			break;
		}
	}
	MenuInstance->menu = menu;
}

void Menu::DrawCursor()
{
	sprintf(bufferD," ");
	int i = 0;
	for (;i<MenuInstance->currentMenu->Count-1;i++)
	{
		DisplayInstance->SetCursorPosition(i+1,0,DISPLAY_SOURCE_LOCAL);
		if (i == MenuInstance->cursor)
		{
			sprintf(bufferD,">");
			DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
			sprintf(bufferD," ");
		}
		else
		{
			DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
		}
	}
}

void Menu::SetSollwert(float sollwert)
{
	MenuInstance->sollwert = sollwert;
	if(MenuInstance->menu == MENU_SOLLWERT)
	{
		DisplayInstance->SetCursorPosition(2,0,DISPLAY_SOURCE_LOCAL);
		sprintf(bufferD," Soll: %2.2f",sollwert);
		DisplayInstance->SendString(bufferD,DISPLAY_SOURCE_LOCAL);
	}
}

