
#include "p33FJ128MC802.h"

int voltaj = 0;

int vit_mot2 = 0;

int auxMotor2 = 0;

void delay1()
{
	int i=0;
	int val=10000;
	val=(val*10)/(1+voltaj);
	for(i=0; i<val; ++i)
	{
		
	}
}

void delay2()
{
	int i=0;
	int val=10000;
	val=(val*10)/(1+vit_mot2);
	for(i=0; i<val*10; ++i)
	{
		
	}
}

void initADC(void)
{
	AD1CON1bits.AD12B = 1; // conversie pe 12 biti
	AD1CON1bits.FORM = 0; //rezultat conversie integer
	AD1CON1bits.SSRC = 2; //timer 3 ca start conversie
	AD1CON1bits.ASAM = 1; // noua esantionare dupa terminarea celei actuale
	AD1CON2bits.CSCNA = 1;//scaneaza intrarile pe CH0+ in timpul achizitiei A
	AD1CON2bits.CHPS = 0; // converteste doar CH0
	AD1CON2bits.SMPI = 0; // incrementeaza adresa DMA dupa terminarea fiecarei conversii
	AD1CON3bits.ADRC = 0; // foloseste ceasul magistralei
	AD1CON3bits.ADCS = 63; // Timpul necesar unei conversii este de 22.4 us
	// Ceasul pentru conversia AD are formula Tad=Tcy*(adcs+1)
	// Tad=Tcy*(adcs+1)=(1/40)*64=1.6us
	// Se seteaza intrarile analogice
	AD1CSSLbits.CSS5 = 1; // Selectam intrarea analogica AN5(RB3) pentru a fi scanata
	// Scriem registrul de configurare al portului
	// Se va folosi doar registrul low al portului de configurare deoarecedsPIC33fj128MC802
	// nu are implementati mai mult de 6 pini pentru ADC
	AD1PCFGL=0xFFFF; // Setam toti pinii portului ADC1 pe modul digital,
	// si activeaza citirea la intrarea portului
	AD1PCFGLbits.PCFG5 = 0;// Setam pinul AN5(RB3) pe intrare analogica,    
	// ADC verifica voltajele pe acel pin (achizitie AD)
	IFS0bits.AD1IF = 0; // Reseteaza flag-ul intreruperii convertorului AD
	IPC3bits.AD1IP = 6; // Seteaza prioritatea intreruperii convertorului AD
	IEC0bits.AD1IE = 1; // Permite intreruperea convertorului AD
	AD1CON1bits.ADON = 1; // setam pentru modul de operare = ON
}
// Timer-ul 3 este setat sa starteze conversia AD la fiecare 0.5 secunde  
void initTmr32()
{	T3CONbits.TON = 0; // Oprire operatii pe 16 biti a Timer-ului 3
	T2CONbits.TON = 0; // Oprire operatii pe 16 / 32 biti a Timer-ului 2
	T2CONbits.T32 = 1; // Validare mod pe 32 biti
	T2CONbits.TCS = 0; // Selectare ceas inter
	T2CONbits.TGATE = 0; // Nu se valideaza modul „gated”
	T2CONbits.TCKPS = 0b01; // Selectare prescaler 1:8
	TMR3 = 0x0000; // Resetare numarator (msw)
	TMR2 = 0x0000; // Resetare numarator (lsw)
	PR3 = 0x0003; // Setare perioada de numarat
	PR2 = 0x83A8; // Setare perioada de numarat
	IFS0bits.T3IF = 0; // Resetare flag de intrerupere
	IEC0bits.T3IE = 1; // Validare intrerupere
	T2CONbits.TON = 1; // Pornire ceas
}

// rutina de tratare a intreruperii convertorului AD
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
	
	voltaj=(ADC1BUF0*3.3)/4095;	//tensiunea achizitionata de la potentiometru

	IFS0bits.AD1IF = 0; // Achita intreruperea convertorului AD
}

/* Rutina de tratare a intreruperii pentru INT0 (S2) */
void __attribute__ ((interrupt, no_auto_psv)) _INT0Interrupt( void )
{
	
	if ( vit_mot2 == 0){vit_mot2 = 10;}
		else {vit_mot2 = vit_mot2 * 1.5;}   
    PR4 = PR4- vit_mot2;
	IFS0bits.INT0IF = 0; // Resetare flag de intrerupere
    
}

void initTmr1 (){
	T1CON = 0;
	IFS0bits.T1IF = 0; // Resetare flag de intrerupere
	IPC0bits.T1IP = 1; // Setare nivel de prioritate
	IEC0bits.T1IE = 1; // Validare intrerupere
	T1CONbits.TCS = 0; // Selectare ceas intern
	T1CONbits.TGATE = 0; // Nu se valideaza modul „gated”
	T1CONbits.TCKPS = 0b10; // Selectare prescaler 1:64
	TMR1= 0x0000; // Resetare numarator
	PR1 = 23031; //setare valoare perioada de numarat;
	T1CONbits.TON = 1; // pornire timer
	
}

void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt( void )
{
	PORTBbits.RB2 =~ PORTBbits.RB2;	//_RB2=~_RB2; // !!!!!!!!!!!!
//	PORTBbits.RB15 =~ PORTBbits.RB15;
	/* Codul rutinei de tratare a intreruperii */	
	IFS0bits.T1IF = 0; // Resetare flag de intrerupere
}

void initTmr4 (){
	T4CON = 0;
	IFS1bits.T4IF = 0; // Resetare flag de intrerupere
	IPC6bits.T4IP = 1; // Setare nivel de prioritate
	IEC1bits.T4IE = 1; // Validare intrerupere
	T4CONbits.TCS = 0; // Selectare ceas intern
	T4CONbits.TGATE = 0; // Nu se valideaza modul „gated”
	T4CONbits.TCKPS = 0b10; // Selectare prescaler 1:64
	TMR4= 0x0000; // Resetare numarator
	PR4 = 23031; //setare valoare perioada de numarat;
	T4CONbits.TON = 1; // pornire timer
	
}

void resetarePiniRB11RB8()
{
	_RB11 = 0;
	_RB10 = 0;
	_RB9 = 0;
	_RB8 = 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _T4Interrupt( void )
{
	if(auxMotor2 == 0)
	{
		resetarePiniRB11RB8();
		_RB11 = 1;
		auxMotor2=auxMotor2+1;
	}
	else if(auxMotor2 == 1)
	{
		resetarePiniRB11RB8();
		_RB10 = 1;
		auxMotor2=auxMotor2+1;
	}
	else if(auxMotor2 == 2)
	{
		resetarePiniRB11RB8();
		_RB9 = 1;
		auxMotor2=auxMotor2+1;
	}
	else if(auxMotor2 == 3)
	{
		resetarePiniRB11RB8();
		_RB8 = 1;
		auxMotor2=0;
	}
	/* Codul rutinei de tratare a intreruperii */	
	IFS0bits.T1IF = 0; // Resetare flag de intrerupere
}



void resetarePiniRB15RB12()
{
	_RB15 = 0;
	_RB14 = 0;
	_RB13 = 0;
	_RB12 = 0;
}

void motor1()
{
	
	while(1)
	{
		if(voltaj!=0)
		{
			_RB15 = 1;
			delay1();
			resetarePiniRB15RB12();
			_RB14 = 1;
			delay1();
			resetarePiniRB15RB12();
			_RB13 = 1;
			delay1();
			resetarePiniRB15RB12();
			_RB12 = 1;
			delay1();
			resetarePiniRB15RB12();
		}
	}
}










