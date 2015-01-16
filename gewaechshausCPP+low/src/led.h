
#include "stm32f4_discovery.h"

class Led {

public:
	unsigned long LED_PINS;
	Led(void);
	~Led(void);
	void On(Led_TypeDef i);
	void Off(Led_TypeDef i);
};
