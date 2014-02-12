#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "ILI9325C_tft.h"
#include "UTouch.h"

const uint16_t PRESS_DELAY_TIME_MS = 100;

enum PRESS_TYPE {
	NOT_PRESS,
	PRESSED,
	RELEASED
};

class Point_t {
private:
	int x;
	int y;
public:
	Point_t(int x = 0,int y = 0) {this->x = x; this->y = y;}
	void setX(int x_) {x = x_;}
	void setY(int y_) {y = y_;}
	int getX(){return x;}
	int getY(){return y;}
};

class Box_t : public Point_t {
private:
	ILI9325C_tft * pScreen;
	int width;
	int height;
	int think;
	int primFrameColor;
	int secFrameColor;
	uint8_t frameColorState;
public:
	Box_t(	ILI9325C_tft * screenP_,
		int x_ = 0, int y_ = 0,
		int w_ = 0, int h_ = 0, int think_ = 1,
		int primFrameColor_ = VGA_BLUE, int secFrameColor_ = VGA_GREEN
		) : Point_t(x_, y_)
	{
		pScreen = screenP_;
		width = w_;
		height = h_;
		think = think_;
		primFrameColor = primFrameColor_;
		secFrameColor = secFrameColor_;
	}
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getThink() { return think; }
	void draw();
	void draw(int color);
	int getPrimFrameColor() { return primFrameColor; }
	int getSecFrameColor() { return secFrameColor; }
};

class Press_t
{
private:
	UTouch * pTouch;
	uint32_t prevPressTime;
	PRESS_TYPE prevPressState;
	PRESS_TYPE pressEvent;
	boolean reqClear;
	void refresh();
public:
	Press_t(UTouch * pTouch_) {
		pTouch = pTouch_;
		prevPressTime = 0;
		prevPressState = RELEASED;
		pressEvent = NOT_PRESS;
		reqClear = false;
	}
	boolean getPressEvent(PRESS_TYPE press);
};

#endif

