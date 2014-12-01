/* ARM Programmierung an der Beuth-Hochschule f�r Technik Berlin
 * Owner: Ren� Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"

class Stepper {

public:
	Stepper();
	void EnableSingelton(void);
	void Init(void);
	void RunStep();
	void Right(int stepps,int time);
	void Left(int stepps,int time);
	void InitTim2(int prescaler, int period);

};

extern Stepper *StepperInstance;
