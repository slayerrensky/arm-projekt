/* ARM Programmierung an der Beuth-Hochschule für Technik Berlin
 * Owner: René Galow and Dennis Rump
 *
 */


#include "stm32f4xx.h"

class Fassade {

public:
	Fassade(int type);
	void InitGewaechshaus(void);
	void Window2Position(int inProzent);
	void RegelungFenster(void);
	void SendMassageToDisplay(char* massage);
	void TerminalDisplayTemp();
	void UpdateDisplayValues();
	int type;
};

extern Fassade *FassadeInstance;
								//  { 0, 1, 2, 3, 4,  5,  6,  7,  8, 9,  10},  // Abweichung in °C
static int TempReglungLUT[2][11] = {{ 0,10,20,40,70,100,100,100,100,100,100},  // Fensterstellung in %
								    { 0, 0, 0, 0, 0,  0, 20, 30, 50, 70,100}}; // Lüftergeschwindigkeit in %
