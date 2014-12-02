/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"
#define STEPPER_USABLE 1
#define STEPPER_BUSSY 0

class Stepper {

public:
	Stepper();
	void EnableSingelton(void);
	void Init(void);
	void RunStep();
	void Right(int stepps,int time);
	void Left(int stepps,int time);
	void Leerlauf();
	void InitTim2(int prescaler, int period);
	int StepperStatus = STEPPER_USABLE;

};

extern Stepper *StepperInstance;
