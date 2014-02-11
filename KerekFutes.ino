// UTouch_ButtonTest (C)2010-2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a quick demo of how create and use buttons.
//
// This program requires the UTFT library.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin 
// numbers in the setup.
//


#include "UTouch.h"
#include "Fonts.h"
#include "ILI9325C_tft.h"
#include "UTFT_Buttons.h"

// Declare which fonts we will be using
//extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];
extern uint8_t SmallFont[];
extern uint8_t DotMatrix_M[];
//extern uint8_t Ubuntu[];

// ILI9325C kijelzo bekotese:
// 1.  GND	-	GND
// 2.  +5V	-	külso 5V szukseges
// 3.  NC
// 4.  RS	-	PC_6
// 5.  RW	-	PC_5
// 6.  RD	-	+3.3V
// 7.  DB0	-	PB_0
// 8.  DB1	-	PB_1
// 9.  DB2	-	PB_2
// 10. DB3	-	PB_3
// 11. DB4	-	PB_4
// 12. DB5	-	PB_5
// 13. DB6	-	PB_6
// 14. DB7	-	PB_7
// 15. CS	-	PC_7
// 16. NC
// 18. RST	-	PC_4
//UTFT(byte model, int RS, int WR,int CS, int RST, int SER=0);
ILI9325C_tft  myGLCD(ILI9325C, PC_6, PC_5, PC_7, PC_4);

//UTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
UTouch myTouch(PA_6, PA_5, PA_4, PA_3, PA_2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);


int x, y;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";

//
//Welcome screen varakozasai ido msec
const unsigned int WELCOME_SRC_TIMEOUT = 1000;


/*************************
**  Required functions  **
*************************/

void setup()
{
	// Initial setup
	myGLCD.InitLCD();
	myGLCD.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	
	welcomeScreen();

	myGLCD.setFont(SmallFont);
	myGLCD.clrScr();
	myButtons.setTextFont(BigFont);
}

/*************************
**   Custom functions   **
*************************/
void welcomeScreen()
{
	myGLCD.setFont(DotMatrix_M);

	myGLCD.setColor(VGA_YELLOW);
	myGLCD.print("TYRE HEATING", CENTER, 60);
	myGLCD.print("SYSTEM", CENTER, 120);
	myGLCD.drawRect(0,200,319,210);

	myGLCD.setFont(SmallFont);
	myGLCD.setColor(VGA_OLIVE);
	myGLCD.print("Made by kovNET",LEFT,227);
	myGLCD.print("Version 0.1", RIGHT, 227);

	myGLCD.setColor(VGA_BLUE);
	for(int i = 1; i < 319; i += 2)
	{
		myGLCD.drawLine(i,202,i,208);
		delay(20);
	}
	delay(WELCOME_SRC_TIMEOUT);
	myGLCD.clrScr();

	tone(TONE_PIN, 1000, 50);
}


void loop()
{
	int but1, pressed_button;
	boolean default_colors = true;

	but1 = myButtons.addButton( 10,  20, 300,  30, "Button 1");

	unsigned long prevTime = 0;

	myButtons.drawButtons();

	myGLCD.print("You pressed:", 110, 205);
	myGLCD.setColor(VGA_BLACK);
	myGLCD.setBackColor(VGA_WHITE);
	myGLCD.print("None", 110, 220);

	while(1) 
	{	
		if (myTouch.dataAvailable() == true && millis() > prevTime)
		{
			prevTime = millis() + 100;
			pressed_button = myButtons.checkButtons();

			if (pressed_button==but1)
			{
				tone(TONE_PIN, 2500, 100);

				myGLCD.print("Button 1", 110, 220);
			}
			else
			{
				myGLCD.print("None    ", 110, 220);
			}
		}
	}
}

