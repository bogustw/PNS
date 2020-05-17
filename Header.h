#ifndef __Header_H__
#define __Header_H__

// External Functions
extern void initADC(void);
extern void initTmr32();
extern void __attribute__((__interrupt__)) _ADC1Interrupt(void);
extern void __attribute__ ((interrupt, no_auto_psv)) _INT0Interrupt( void );
extern void initTmr1 ();
extern void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt( void );
extern void delay1();
extern void delay2();
extern void resetarePiniRB11RB8();
extern void resetarePiniRB15RB12();
extern void motor1();

#endif

