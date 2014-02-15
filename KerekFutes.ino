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

#include <itoa.h>
#include "UTouch.h"
#include "Fonts.h"
#include "ILI9325C_tft.h"
#include "buttons.h"


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
//UTFT_Buttons  myButtons(&myGLCD, &myTouch);


int x, y;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";

//
//Welcome screen varakozasai ido msec
const unsigned int WELCOME_SRC_TIMEOUT = 1;

//
// Allando nyomogombok
//

//Homerseklet kijelzo gombok
const int FOREGROUND = VGA_WHITE;
const int BACKGROUND = VGA_BLUE;
const int BUTTON_NORM_COLOR = VGA_BLUE;
const int BUTTON_INV_COLOR = VGA_RED;

//Ennyi parnank van
const uint8_t MAX_TAGNUMBER = 3; 
const uint8_t ALL_NUMBERS = 4;

const int tempsButtY = 30;
const uint32_t menuTimeout = 10000; //msec

Button_t temperature1(&myGLCD, &myTouch,
	0, 0, 110, 59, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "`C", CENTER, tempsButtY, 1);

Button_t temperature2(&myGLCD, &myTouch,
	0, 60, 110, 59, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "`C", CENTER, tempsButtY, 1);

Button_t temperature3(&myGLCD, &myTouch,
	0, 120, 110, 59, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "`C", CENTER, tempsButtY, 1);

Button_t temperature4(&myGLCD, &myTouch,
	0, 180, 110, 59, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "`C", CENTER, tempsButtY, 1);

//A beallitott homerseklet dobozok
Box_t setTemperature1(&myGLCD,
	65, 0, 45, 20, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, SmallFont, "`C", CENTER, CENTER, 1);

Box_t setTemperature2(&myGLCD,
	65, 60, 45, 20, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, SmallFont, "`C", CENTER, CENTER, 1);

Box_t setTemperature3(&myGLCD,
	65, 120, 45, 20, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, SmallFont, "`C", CENTER, CENTER, 1);

Box_t setTemperature4(&myGLCD,
	65, 180, 45, 20, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, SmallFont, "`C", CENTER, CENTER, 1);


//Homersekleti erteket kijelzo nyomogombok mutato tombje
Button_t * temperatures[] = {
	&temperature1,
	&temperature2,
	&temperature3,
	&temperature4
};

//A beallitott homersekleti dobozok tombje
Box_t * setTemperatures[] = {
	&setTemperature1,
	&setTemperature2,
	&setTemperature3,
	&setTemperature4
};
 
const uint8_t START_STATUS = 0;
const uint8_t STOP_STATUS = 1;
const uint8_t ELAPSED_TIME = 0;
const uint8_t REMAIN_TIME = 1;

//START/STOP gomb
Button_t startButton(&myGLCD, &myTouch,
	120, 140, 190, 90, 4,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "START", CENTER, CENTER, 1);

//TIME button
Button_t timeButton(&myGLCD, &myTouch,
	120, 48, 190, 70, 1,
	FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
	FOREGROUND, DotMatrix_M, "00:00", CENTER, CENTER, 1);

class EnvironmentVars_S {
public:
	uint8_t temperatures[4];
	uint8_t setTemperatures[5];
	//uint8_t timeType;
	uint32_t startTime;
	uint32_t timeRawSec;
	uint8_t sec;
	uint16_t min;
	uint8_t startStopStatus;
	uint8_t timeMode;
	boolean timeRefreshing;
	EnvironmentVars_S()
	{
		for(uint8_t i = 0; i < 4; i++) {
			temperatures[i] = 0;
			setTemperatures[i] = 80;
		}
		setTemperatures[ALL_NUMBERS] = 80;
		startTime = 0;
		timeRawSec = 0;
		sec = 0;
		min = 0;
		startStopStatus = STOP_STATUS;
		timeMode = ELAPSED_TIME;
		timeRefreshing = false;
	}
};

EnvironmentVars_S envVars;

/*************************
**  Required functions  **
*************************/

#define DEBUGLEVEL 1

void setup()
{
#if DEBUGLEVEL > 0
	Serial.begin(115200);
#endif
	Serial.println(sizeof(unsigned long));
	Serial.println(sizeof(unsigned long long));
	Serial.println(sizeof(unsigned int));
	Serial.println(sizeof(unsigned char));
	// Initial setup
	myGLCD.InitLCD();
	myGLCD.clrScr();

	myTouch.InitTouch();
//	myTouch.setPrecision(PREC_MEDIUM);
	myTouch.setPrecision(PREC_EXTREME);
	welcomeScreen();

	myGLCD.setFont(SmallFont);
	myGLCD.clrScr();
	

	drawScreen();
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
		delay(1);
	}
	delay(WELCOME_SRC_TIMEOUT);
	myGLCD.clrScr();

	tone(TONE_PIN, 1000, 50);
}

void loop()
{
	if(millis() > envVars.timeRawSec && envVars.startStopStatus == START_STATUS) {
		envVars.timeRawSec = millis() + 1000;
		uint32_t sec;
		sec = (millis() - envVars.startTime) / 1000;
		envVars.min = sec / 60;
		envVars.sec = sec - (envVars.min * 60);

		if(sec == (999*60+59)) {
			envVars.startStopStatus = STOP_STATUS;
			drawStartStopB(STOP_STATUS);
		}

		//Frissitettunk
		envVars.timeRefreshing = true;
	}

	//Homersekleti beallito gombok lekerdezese
	for(uint8_t i = 0; i < 4; i++) {
		PRESS_TYPE press = temperatures[i]->getButtonEvent();
		if(press == RELEASED || press == OTHER_RELEASED) {
			//Belepunk a temp setting menube
			if(press == RELEASED) {
				temperatureSetting(i);
			}
			drawNumbers(i);
			drawSetTemperatures(setTemperatures[i], i);
		}
	}

	//Start/Stop gomb lekerdezese
	if(startButton.getButtonEvent() == RELEASED) {
		if(envVars.startStopStatus == STOP_STATUS) {
			//Idozitos inditasa
			envVars.startTime = millis();
			envVars.startStopStatus = START_STATUS;
			drawStartStopB(START_STATUS);

		} else {
			envVars.startStopStatus = STOP_STATUS;
			drawStartStopB(STOP_STATUS);
		}
	}

	//Ha frissult az ido akkor frissitjuk a time buttonon is
	if(envVars.timeRefreshing) {
		envVars.timeRefreshing = false;
		refreshTime();
	}

}

//A kepernyo ujrarajzolasa
void drawScreen() {
	for(uint8_t i = 0; i < 4; i++) {
		drawTemperatures(temperatures[i]);
		drawSetTemperatures(setTemperatures[i], i);
	}
	drawNumbers(ALL_NUMBERS);
	drawElapsedTime();
	drawStartStopB(envVars.startStopStatus);
	drawTimeB();
}

//Felugro ablak a homerseklet beallitasara
void temperatureSetting(uint8_t tagNumber) {
	//Felugro ablak definicioja
	//Keret
	Box_t frame(&myGLCD,
		20, 20, 280, 200, 3,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, DotMatrix_M, "`C", CENTER, CENTER, 1);
	Box_t temperatureText(&myGLCD,
		115, 30, 180, 20, 1,
		BACKGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "Temperature", CENTER, CENTER, 1);
	Box_t settingText(&myGLCD,
		115, 56, 115, 20, 1,
		BACKGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "Setting", CENTER, CENTER, 1);	

	frame.setText(envVars.setTemperatures[tagNumber], "`C");
	frame.drawNormal();
	temperatureText.drawNormal();
	settingText.drawNormal();

	Button_t tyreNumButton(&myGLCD, &myTouch,
		25, 25, 80, 50, 1,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, ".", CENTER, CENTER, 1);
	Button_t decButton(&myGLCD, &myTouch,
		30, 160, 80, 50, 1,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "-", CENTER, CENTER, 1);
	Button_t incButton(&myGLCD, &myTouch,
		120, 160, 80, 50, 1,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "+", CENTER, CENTER, 1);
	Button_t okButton(&myGLCD, &myTouch,
		210, 160, 80, 50, 1,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "OK", CENTER, CENTER, 1);

	tyreNumButton.setText(tagNumber + 1, "");
	tyreNumButton.drawNormal();

	incButton.drawNormal();
	decButton.drawNormal();
	okButton.drawNormal();
	
	uint8_t prevData = 0;
	uint32_t startTime = millis() + menuTimeout;

	EnvironmentVars_S tempEnv;

	for(uint8_t i = 0; i <=ALL_NUMBERS; i++) {
		tempEnv.setTemperatures[i] = envVars.setTemperatures[i];
	}

	while(true) {
		if(tyreNumButton.getButtonEvent() == RELEASED) {
			startTime = millis() + menuTimeout;
			tagNumber++;
			if(tagNumber <= MAX_TAGNUMBER) {
				tyreNumButton.setText(tagNumber + 1, "");
			} else if(tagNumber == ALL_NUMBERS) {
				tyreNumButton.setText("ALL");
			} else {
				tagNumber = 0;
				tyreNumButton.setText(tagNumber + 1, "");
			}
		}

		PRESS_TYPE incB = incButton.getButtonEvent();
		PRESS_TYPE decB = decButton.getButtonEvent();
		PRESS_TYPE okB = okButton.getButtonEvent();

		if(incB == RELEASED ) {
			startTime = millis() + menuTimeout;
			if(tempEnv.setTemperatures[tagNumber] < 99)
				tempEnv.setTemperatures[tagNumber]++;
		}
		else if(decB == RELEASED ) {
			startTime = millis() + menuTimeout;
			if(tempEnv.setTemperatures[tagNumber] > 0)
				tempEnv.setTemperatures[tagNumber]--;
		}
		else if(okB == RELEASED) {
			if(tagNumber == ALL_NUMBERS) {
				for(uint8_t i = 0; i <= MAX_TAGNUMBER ; i++) {
					envVars.setTemperatures[i] = tempEnv.setTemperatures[ALL_NUMBERS];
					setTemperatures[i]->setText(envVars.setTemperatures[i], "`C");
				}
				envVars.setTemperatures[ALL_NUMBERS] = tempEnv.setTemperatures[ALL_NUMBERS];
			} else {
				envVars.setTemperatures[tagNumber] = tempEnv.setTemperatures[tagNumber];
				setTemperatures[tagNumber]->setText(envVars.setTemperatures[tagNumber], "`C");
			}
			myGLCD.clrScr();
			drawScreen();
			break;
		}
		else if(millis() > startTime) {
			myGLCD.clrScr();
			drawScreen();
			break;
		} else {
			if(prevData != tempEnv.setTemperatures[tagNumber]) {
				frame.setText(tempEnv.setTemperatures[tagNumber], "`C");
				prevData = tempEnv.setTemperatures[tagNumber];
			}
		}
	}
}

//Kirajzoljuk a szamokat
void drawNumbers(const uint8_t whichNumber) {
	if(whichNumber == 0 || whichNumber == ALL_NUMBERS) {
		Box_t num1(&myGLCD, 0, 0, 20, 20, 1,
			FOREGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
			FOREGROUND, BigFont, "1", CENTER, CENTER, 1);
		num1.drawNormal();
	}

	if(whichNumber == 1 || whichNumber == ALL_NUMBERS) {
		Box_t num2(&myGLCD, 0, 60, 20, 20, 1,
			FOREGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
			FOREGROUND, BigFont, "2", CENTER, CENTER, 1);
		num2.drawNormal();
	}

	if(whichNumber == 2 || whichNumber == ALL_NUMBERS) {
		Box_t num3(&myGLCD, 0, 120, 20, 20, 1,
			FOREGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
			FOREGROUND, BigFont, "3", CENTER, CENTER, 1);
		num3.drawNormal();
	}

	if(whichNumber == 3 || whichNumber == ALL_NUMBERS) {
		Box_t num4(&myGLCD, 0, 180, 20, 20, 1,
			FOREGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
			FOREGROUND, BigFont, "4", CENTER, CENTER, 1);
		num4.drawNormal();
	}
}

//Kiirjuk a hofok eretekeket
void drawTemperatures(Button_t * temperatures) {
	temperatures->drawNormal();	
}

//Kirajzoljuk a beallitott homersekleti ertekeket
void drawSetTemperatures(Box_t * setTemperatures, uint8_t tagNumber) {
	setTemperatures->setText(envVars.setTemperatures[tagNumber], "`C");
	setTemperatures->drawNormal();
}

//Kirajzoljuk az Elpsed Time dobozt. Ebben van a start/stop es a Time
//gomb illetve felirat is.
void drawElapsedTime() {
	Box_t elapsedTime(&myGLCD,
		111, 0, 208, 239, 1,
		FOREGROUND, FOREGROUND, BUTTON_NORM_COLOR, BUTTON_INV_COLOR,
		FOREGROUND, BigFont, "Time elapsed", CENTER, 20, 1);

	elapsedTime.drawNormal();
}

//Kirajzoljuk a start/stop buttont
void drawStartStopB(uint8_t status) {
	if(status == STOP_STATUS) {
		startButton.setNormBackgroundColor(BUTTON_NORM_COLOR);
		startButton.setInvBackgroundColor(BUTTON_INV_COLOR);
		startButton.setText("START");
	} else {
		startButton.setNormBackgroundColor(BUTTON_INV_COLOR);
		startButton.setInvBackgroundColor(BUTTON_NORM_COLOR);
		startButton.setText("STOP");
	}
	startButton.drawNormal();
}

//TimeButton kirajzolasa
void drawTimeB() {
	//Elkerjuk az aktualis idoadatokat
	refreshTime();
	timeButton.drawNormal();
}

//A time button idoadatanak frissitese
void refreshTime() {
	char s[8];
	getTimeStr(s);

	timeButton.setText(s, false);
}

//Az atadott stringe rakja az aktualis envVar idoadatot
void getTimeStr(char * s) {
	String s_min(envVars.min);
	if(envVars.min < 10)
		s_min = "0" + s_min;

	String s_sec(envVars.sec);
	if(envVars.sec < 10)
		s_sec = "0" + s_sec;
	String concat_s = s_min + ":" + s_sec;
	concat_s.toCharArray(s,7);
}






