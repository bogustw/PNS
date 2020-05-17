

#include "p33FJ128MC802.h"
#include "Header.h"





// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC);
// Enable Clock Switching and Configure
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF); // FRC + PLL
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT); // XT + PLL
_FWDT(FWDTEN_OFF); // Watchdog Timer Enabled/disabled by user software

void main(void){

	_RB2 = 0;
	_RB15 = 1;
	TRISB = 0x0000;
	_TRISB7 = 1;
	_INT0IF = 0;
	_INT0IE = 1;
	_INT0EP = 1;
	initTmr1();
	initTmr32();
	resetarePiniRB15RB12();
	motor1();
//    while(in)

}