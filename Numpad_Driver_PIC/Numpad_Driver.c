/*-----------------------------------------------------------------------------
	File Name:	LCD_Driver.c
	Author:		Aswin Sreeraj
	Date:		02/16/2025
	Modified:	None

	Description: 	
-----------------------------------------------------------------------------*/

/* Preprocessor ---------------------------------------------------------------
   Hardware Configuration Bits ----------------------------------------------*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ------------------------------------------------------------------
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <string.h>

// Constants  -----------------------------------------------------------------
#define TRUE	1	
#define FALSE	0
#define LCDPORT LATD
#define RS LATCbits.LATC4
#define E LATCbits.LATC5
#define R1 LATAbits.LATA7
#define R2 LATAbits.LATA6
#define R3 LATAbits.LATA5
#define R4 LATAbits.LATA4
#define C1 PORTAbits.RA3
#define C2 PORTAbits.RA2
#define C3 PORTAbits.RA1
#define C4 PORTAbits.RA0


// Functions  -----------------------------------------------------------------

/*>>> configOSC: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Set OSC module to 16 MHz and wait for the frequency to be stable
Input: 		None
Returns:	None
 ----------------------------------------------------------------------------*/
void configOSC(void)
{
	OSCCON = 0x72;	// Set the required value of OSCCON register
	while(!OSCCONbits.HFIOFS);	// Wait for frequency to get stable

} // eo configOSC::


/*>>> configurePorts: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Configure ports for data type, flow direction and intialization
Input: 		None
Returns:	None
 ----------------------------------------------------------------------------*/
void configurePorts(void)
{
	ANSELC = 0x00;
	LATC = 0x00;
	TRISC = 0x00;
	ANSELD = 0x00;
	LATD = 0x00;
	TRISD = 0x00;
	ANSELA = 0x00;
	TRISA = 0x0F;
} // eo configurePorts::


/*>>> lcdCommand: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Send a command to the lcd module
Input: 		unsigned char command, the command to be send
Returns:	None
 ----------------------------------------------------------------------------*/
void lcdCommand(unsigned char command)
{
	RS = 0;
	LCDPORT = command;
	E = 1;
	Delay1KTCYx(2);
	E = 0;
	Delay1KTCYx(2);
}	// eo lcdCommand::

/*>>> lcdInitialize: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Initialize the lcd module for display
			> 5x10 display
			> 2 lines
Input: 		None
Returns:	None
 ----------------------------------------------------------------------------*/
void lcdInitialize()
{
	E = 0;
	RS = 0;
	LCDPORT = 0;
	TRISD = 0;
	TRISC = 0;

	
	lcdCommand(0x3C); // Function set, DL(8 bit) = 1, N(2 lines) = 1, F(5x11 font) = 1
	lcdCommand(0x0C); // Display O/F, D (turn on) = 1, C(cursor) = 0, B(blink) = 0
	lcdCommand(0x01); // Clear Display
	lcdCommand(0x07); // Entry mode set, I/D(increment cursor) = 1, S(shift) = 0
	lcdCommand(1);	  // Clear Display
	Delay1KTCYx(2);	  // Return Home
	lcdCommand(0x80); // Cursor to first line
}	// eo lcdInitialize::

/*>>> lcdData: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Send character data to the lcd module for display
Input: 		char data, the data to be send
Returns:	None
 ----------------------------------------------------------------------------*/
void lcdData(char data)
{
	RS = 1;
	LCDPORT = data;
	E = 1;
	Delay1KTCYx(2);
	E = 0;
	Delay1KTCYx(2);
}	// eo lcdData::

/*>>> lcdString: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		05/12/2023
Modified:	None
Desc:		Send a string to the lcd module
Input: 		char * stringLCD, the string to be send to the module
Returns:	None
 ----------------------------------------------------------------------------*/
void lcdString(char *stringLCD)
{
	while(*stringLCD)
	{
		lcdData(*(stringLCD++));
	}
}	//	eo lcdString::


/*>>> lcdString: -----------------------------------------------------------
Author:		Aswin Sreeraj
Date:		02/17/2025
Modified:	None
Desc:		Scan the keypad for inputs
Input: 		None
Returns:	char, return the character according to button
 ----------------------------------------------------------------------------*/
char keypadScanner()
{
    R1 = 0; R2 = 0; R3 = 0; R4 = 0;  // Reset all rows

    R1 = 1;
    Delay1KTCYx(40); // ~10ms debounce
    if (C1) { while (C1); return '1'; }
    if (C2) { while (C2); return '2'; }
    if (C3) { while (C3); return '3'; }
    if (C4) { while (C4); return 'A'; }
    R1 = 0;

    R2 = 1;
    Delay1KTCYx(40);
    if (C1) { while (C1); return '4'; }
    if (C2) { while (C2); return '5'; }
    if (C3) { while (C3); return '6'; }
    if (C4) { while (C4); return 'B'; }
    R2 = 0;

    R3 = 1;
    Delay1KTCYx(40);
    if (C1) { while (C1); return '7'; }
    if (C2) { while (C2); return '8'; }
    if (C3) { while (C3); return '9'; }
    if (C4) { while (C4); return 'C'; }
    R3 = 0;

    R4 = 1;
    Delay1KTCYx(40);
    if (C1) { while (C1); return '*'; }
    if (C2) { while (C2); return '0'; }
    if (C3) { while (C3); return '#'; }
    if (C4) { while (C4); return 'D'; }
    R4 = 0;

    return 'n';  // No key pressed
}


char switchPressScan(void)
{
    char key = 'n';
    while (key == 'n') {
        key = keypadScanner();
        Delay1KTCYx(200);  // ~50ms debounce
    }
    return key;
}



/*--- MAIN: FUNCTION ----------------------------------------------------------
 ----------------------------------------------------------------------------*/
void main( void )
{
	// Variable initialization
	char message[] = "Key pressed";
	char dash[] = "--------------------";
	char operations[] = "A=+, B=-, C=*, D=/";
	char pressMe[] = "Input: ";
	char coperand1[5] = "";
	char coperand2[5] = "";
	char temp[5] = "";
	int result;
	char cresult[5];

	char keyPressed = 'n';	
	// System initialization
	configOSC();
	configurePorts();
	lcdInitialize();
	
	lcdCommand(0x80);	// First line
	lcdString(message);
	lcdCommand(0xC0);	// Second line
	lcdString(dash);
	lcdCommand(0x94);
	lcdString(operations);
	lcdCommand(0xD4);
	while(TRUE)
	{
		keyPressed = switchPressScan();
		if(keyPressed == 'C')
		{
			lcdCommand(0x01);
			lcdCommand(0x80);	// First line
			lcdString(message);
			lcdCommand(0xC0);	// Second line
			lcdString(dash);
			lcdCommand(0x94);
			lcdString(operations);
			lcdCommand(0xD4);
			continue;
		}
		sprintf(temp,"%s%c",temp, keyPressed);
		lcdData(keyPressed);	
		if(keyPressed == 'A')
		{	
			strncpy(coperand1, temp, strlen(temp) - 1);
			strcpy(temp, "");
			while(keyPressed != '#')
			{
				keyPressed = switchPressScan();
				sprintf(temp,"%s%c",temp, keyPressed);
				lcdData(keyPressed);	
			}
			strncpy(coperand2, temp, strlen(temp) - 1);
			strcpy(temp, "");
			result = atoi(coperand1) + atoi(coperand2);
			sprintf(cresult, "%d", result);
			lcdString(cresult);
		}
	}
	

} // eo main::
