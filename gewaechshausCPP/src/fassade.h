/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */


#include "stm32f4xx.h"

class Fassade {

public:
	Fassade();
	void InitGewaechshaus(void);
	void Window2Position(void);
};

extern Fassade *FassadeInstance;
