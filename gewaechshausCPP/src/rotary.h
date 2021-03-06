/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"

class Rotary {

public:
	Rotary();
	void EnableSingelton(void);
	void Init(void);
	int GetNumberFrom2Bool(int a, int b);
	int GetPosition();
	void Process(void);
	bool hasButtonPressd();
	bool resetButtonPressd();
	int istzustand;
	int folgezustand;
	int position;
	bool button = 0;

	//int table[16] = { 0, 1, 0, 1, 1, 0, 2, 0, 2, 0, 0, 2, 0, 2, 1, 0 };
	int table[16] = { 0, 2, 1, 0, 1, 0, 0, 2, 2, 0, 0, 1, 0, 1, 2, 0 };

protected:
	int buttonState;

};

extern Rotary *RotaryInstance;

