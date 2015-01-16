/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */
#include "stm32f4xx.h"
#define STEPPER_USABLE 1
#define STEPPER_BUSSY 0
#define STEPPER_MAX_POSITION 20000
#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 0

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
	void Go2Step(int g);
	int GetPosition();
	int GetPositionInProzent();

	int StepperStatus;
	int currentStep;
	int stepperEnd;
	int direction;
	int position;

};

extern Stepper *StepperInstance;
