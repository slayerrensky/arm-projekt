#define AMPERMETER_H_

#ifdef AMPERMETER_H_

#include "stm32f4xx.h"

class Ampermeter {
public:
	Ampermeter();
	float getCurrent(void);
};

extern Ampermeter *AmpermeterInstance;

#endif
