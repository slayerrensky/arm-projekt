/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"

#define MENU_MAIN 0
#define MENU_SENSOREN 1
#define MENU_SOLLWERT 2
#define MENU_SUBMIT 3

typedef struct {
	int menu;
	int Count;
    char *menuEntrys;
}menu_str;

extern bool menuShow;

class Menu {

public:
	Menu();
	void Init(void);
	void DisplayMenu(int menu);
	void MenuUP(void);
	void PrintMenu(menu_str *menu);
	void Cursor(int i);
	void Submit();
	void DrawCursor();
	void SetSollwert(float sollwert);
	//int menuShow = false;
	int menu=0;
	int menuBefore=0;
	menu_str *currentMenu;
	int cursor=0;
	float sollwert = 0;


};

extern Menu *MenuInstance;

