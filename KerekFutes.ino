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
#include "UTouch.h"

// Declare which fonts we will be using
//extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t DotMatrix_M[];
extern uint8_t SmallFont[];
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


int x, y;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";


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

	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 255);
	drawButtons();  
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
		delay(10);
	}
	delay(2500);
	myGLCD.clrScr();

	tone(TONE_PIN, 1000, 50);
}

void drawButtons()
{
	// Draw the upper row of buttons
	for (x=0; x<5; x++)
	{
		myGLCD.setColor(0, 0, 255);
		myGLCD.fillRoundRect (10+(x*60), 10, 60+(x*60), 60);
		myGLCD.setColor(255, 255, 255);
		myGLCD.drawRoundRect (10+(x*60), 10, 60+(x*60), 60);
		myGLCD.printNumI(x+1, 27+(x*60), 27);
	}
	// Draw the center row of buttons
	for (x=0; x<5; x++)
	{
		myGLCD.setColor(0, 0, 255);
		myGLCD.fillRoundRect (10+(x*60), 70, 60+(x*60), 120);
		myGLCD.setColor(255, 255, 255);
		myGLCD.drawRoundRect (10+(x*60), 70, 60+(x*60), 120);
		if (x<4)
			myGLCD.printNumI(x+6, 27+(x*60), 87);
	}
	myGLCD.print("0", 267, 87);
	// Draw the lower row of buttons
	myGLCD.setColor(0, 0, 255);
	myGLCD.fillRoundRect (10, 130, 150, 180);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect (10, 130, 150, 180);
	myGLCD.print("Clear", 40, 147);
	myGLCD.setColor(0, 0, 255);
	myGLCD.fillRoundRect (160, 130, 300, 180);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect (160, 130, 300, 180);
	myGLCD.print("Enter", 190, 147);
	myGLCD.setBackColor (0, 0, 0);
}

void updateStr(int val)
{
	if (stCurrentLen<20)
	{
		stCurrent[stCurrentLen]=val;
		stCurrent[stCurrentLen+1]='\0';
		stCurrentLen++;
		myGLCD.setColor(0, 255, 0);
		myGLCD.print(stCurrent, LEFT, 224);
	}
	else
	{
		myGLCD.setColor(255, 0, 0);
		myGLCD.print("BUFFER FULL!", CENTER, 192);
		delay(500);
		myGLCD.print("            ", CENTER, 192);
		delay(500);
		myGLCD.print("BUFFER FULL!", CENTER, 192);
		delay(500);
		myGLCD.print("            ", CENTER, 192);
		myGLCD.setColor(0, 255, 0);
	}
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
	myGLCD.setColor(255, 0, 0);
	myGLCD.drawRoundRect (x1, y1, x2, y2);
	while (myTouch.dataAvailable())
		myTouch.read();
	tone(TONE_PIN, 1047, 30);
	tone(TONE_PIN, 2093, 30);
	tone(TONE_PIN, 4186, 30);

	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect (x1, y1, x2, y2);
}


void loop()
{
	while (true)
	{
		if (myTouch.dataAvailable())
		{
			myTouch.read();
			x=myTouch.getX();
			y=myTouch.getY();

			if ((y>=10) && (y<=60))  // Upper row
			{
				if ((x>=10) && (x<=60))  // Button: 1
				{
					waitForIt(10, 10, 60, 60);
					updateStr('1');
				}
				if ((x>=70) && (x<=120))  // Button: 2
				{
					waitForIt(70, 10, 120, 60);
					updateStr('2');
				}
				if ((x>=130) && (x<=180))  // Button: 3
				{
					waitForIt(130, 10, 180, 60);
					updateStr('3');
				}
				if ((x>=190) && (x<=240))  // Button: 4
				{
					waitForIt(190, 10, 240, 60);
					updateStr('4');
				}
				if ((x>=250) && (x<=300))  // Button: 5
				{
					waitForIt(250, 10, 300, 60);
					updateStr('5');
				}
			}

			if ((y>=70) && (y<=120))  // Center row
			{
				if ((x>=10) && (x<=60))  // Button: 6
				{
					waitForIt(10, 70, 60, 120);
					updateStr('6');
				}
				if ((x>=70) && (x<=120))  // Button: 7
				{
					waitForIt(70, 70, 120, 120);
					updateStr('7');
				}
				if ((x>=130) && (x<=180))  // Button: 8
				{
					waitForIt(130, 70, 180, 120);
					updateStr('8');
				}
				if ((x>=190) && (x<=240))  // Button: 9
				{
					waitForIt(190, 70, 240, 120);
					updateStr('9');
				}
				if ((x>=250) && (x<=300))  // Button: 0
				{
					waitForIt(250, 70, 300, 120);
					updateStr('0');
				}
			}

			if ((y>=130) && (y<=180))  // Upper row
			{
				if ((x>=10) && (x<=150))  // Button: Clear
				{
					waitForIt(10, 130, 150, 180);
					stCurrent[0]='\0';
					stCurrentLen=0;
					myGLCD.setColor(0, 0, 0);
					myGLCD.fillRect(0, 224, 319, 239);
				}
				if ((x>=160) && (x<=300))  // Button: Enter
				{
					waitForIt(160, 130, 300, 180);
					if (stCurrentLen>0)
					{
						for (x=0; x<stCurrentLen+1; x++)
						{
							stLast[x]=stCurrent[x];
						}
						stCurrent[0]='\0';
						stCurrentLen=0;
						myGLCD.setColor(0, 0, 0);
						myGLCD.fillRect(0, 208, 319, 239);
						myGLCD.setColor(0, 255, 0);
						myGLCD.print(stLast, LEFT, 208);
					}
					else
					{
						myGLCD.setColor(255, 0, 0);
						myGLCD.print("BUFFER EMPTY", CENTER, 192);
						delay(500);
						myGLCD.print("            ", CENTER, 192);
						delay(500);
						myGLCD.print("BUFFER EMPTY", CENTER, 192);
						delay(500);
						myGLCD.print("            ", CENTER, 192);
						myGLCD.setColor(0, 255, 0);
					}
				}
			}
		}
	}
}

