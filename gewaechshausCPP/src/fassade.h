/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
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
