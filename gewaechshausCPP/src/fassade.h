/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */


#include "stm32f4xx.h"

class Fassade {

public:
	Fassade(int type);
	void InitGewaechshaus(void);
	void Window2Position(int inProzent);
	void DisplayMassage(char* massage);
	int type;
};

extern Fassade *FassadeInstance;
