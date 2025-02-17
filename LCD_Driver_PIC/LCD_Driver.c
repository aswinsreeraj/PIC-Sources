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

// Constants  -----------------------------------------------------------------
#define TRUE	1	
#define FALSE	0
#define LCDPORT LATD
#define RS LATCbits.LATC4
#define E LATCbits.LATC5


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



/*--- MAIN: FUNCTION ----------------------------------------------------------
 ----------------------------------------------------------------------------*/
void main( void )
{
	// Variable initialization
	char name[] = "Aswin Sreeraj";
	char email[] = "aswinsreeraj96@gmail";
	char phone[] = "(548) 881-2142";
	char copy[] = "Copyright (C)";
	
	// System initialization
	configOSC();
	configurePorts();
	lcdInitialize();
	
	while(TRUE)
	{
		lcdCommand(0x80);	// First line
		lcdString(name);
		lcdCommand(0xC0);	// Second line
		lcdString(email);	
		lcdCommand(0x94);	// Third line
		lcdString(phone);	
		lcdCommand(0xD4);	// Fourth line
		lcdString(copy);	
	}

} // eo main::
