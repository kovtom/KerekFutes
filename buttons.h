#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <itoa.h>
#include "ILI9325C_tft.h"
#include "UTouch.h"

#define DEBUGLEVEL 0

const uint16_t PRESS_DELAY_TIME_MS = 100;

enum PRESS_TYPE {
	NOT_PRESS,
	PRESSED,
	RELEASED,
	OTHER_RELEASED
};

enum BOX_STATUS_TYPE {
	NORMAL,
	INVERT
};

class Point_t {
private:
	int x;
	int y;
protected:
	Point_t(int x = 0,int y = 0) {this->x = x; this->y = y;}
	virtual void setX(int x_) {x = x_;}
	virtual void setY(int y_) {y = y_;}
	virtual int getX(){return x;}
	virtual int getY(){return y;}
};

class Box_t : private Point_t {
private:
	int width;
	int height;
	int think;
	int primFrameColor;
	int secFrameColor;
	uint8_t frameColorState;
	int normBackgroundColor;
	int invBackgroundColor;
	int textColor;
	uint8_t * font;
	char * text;
	int tx, ty;
	uint8_t textLength;
	uint8_t _textLength(char * inpText);
	void _copyText(char * inpText);
	void _setNewText(char * inpText);
	void _clearOldTExt(boolean clear);
	uint8_t priority;
	BOX_STATUS_TYPE status;
protected:
	ILI9325C_tft * pScreen;
public:
	Box_t(	ILI9325C_tft * screenP_,
			int x_ = 0,
			int y_ = 0,
			int w_ = 100,
			int h_ = 60,
			int think_ = 2,
			int primFrameColor_ = VGA_BLUE,
			int secFrameColor_ = VGA_BLUE,
			int normBackgroundColor_ = VGA_BLUE,
			int invBackgroundColor_ = VGA_GREEN,
			int textColor_ = VGA_WHITE,
			uint8_t * font_ = 0,
			char * text_ = 0,
			int tx_ = CENTER,
			int ty_ = CENTER,
			uint8_t priority_ = 0
		);
	~Box_t() { delete [] text; }
	void draw();
	void draw(int color);
	void drawNormal();
	void drawInvert();
	int getBoxX() { return getX(); }
	int getBoxY() { return getY(); }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getThink() { return think; }
	int getPrimFrameColor() { return primFrameColor; }
	int getSecFrameColor() { return secFrameColor; }
	int getNormBackgroundColor() { return normBackgroundColor; }
	int getInvBackgroundColor() { return invBackgroundColor; }
	boolean pointInBox(int x, int y);
	char * getText() { return text; }
	uint8_t * getFont() { return font; }
	void print();
	void setText(char * inpText, boolean clear = true);
	void setText(int number, char * inpText, boolean clear = true);
	void setNormBackgroundColor(int color);
	void setInvBackgroundColor(int color);
};

class Press_t : private Point_t
{
private:
	UTouch * pTouch;
	uint32_t prevPressTime;
	PRESS_TYPE prevPressState;
	PRESS_TYPE pressEvent;
	boolean reqClear;
	void refreshTouchEvent();
public:
	Press_t(){}
	Press_t(UTouch * pTouch_) : Point_t() {
		pTouch = pTouch_;
		prevPressTime = 0;
		prevPressState = RELEASED;
		pressEvent = NOT_PRESS;
		reqClear = false;
	}
	void begin(UTouch * pTouch_);
	boolean getPressEvent(PRESS_TYPE press);
	int getPressX() { return getX();}
	int getPressY() { return getY();}
};

class Button_t : public Box_t , public Press_t 
{
private:
	PRESS_TYPE prevState;
	PRESS_TYPE buttonEvent;
public:
	Button_t(	ILI9325C_tft * pScreen_,
				UTouch * pTouch_,
				int x_,
				int y_ ,
				int width_,
				int height_,
				int think_, 
				int primFrameColor_,
				int secFrameColor_,
				int normBackgroundColor_,
				int invBackgroundColor_,
				int textColor_,
				uint8_t * font_,
				char * text_ = 0,
				int tx_ = 0,
				int ty_ = 0,
				int priority_ = 0
	) : Box_t(
				pScreen_,
				x_,
				y_,
				width_,
				height_,
				think_,
				primFrameColor_,
				secFrameColor_,
				normBackgroundColor_,
				invBackgroundColor_,
				textColor_,
				font_,
				text_,
				tx_,
				ty_,
				priority_
			),
				Press_t(pTouch_)
	{ 
		buttonEvent = NOT_PRESS;
		prevState = NOT_PRESS;
	}
	PRESS_TYPE getButtonEvent();
};
#endif

