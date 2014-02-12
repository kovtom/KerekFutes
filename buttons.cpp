#include "buttons.h"

//Kirajzol egy dobozt az objektumban oldalvastagsaggal
void Box_t::draw()
{
	for(int i = 0; i < think; i++)
	{
		pScreen->drawRect(getX() + i, getY() + i,
			getX() + width - i, getY() + height - i);
	}
}

//Kirajzol egy dobozt a megadott oldalszinnel
void Box_t::draw(int color)
{
	pScreen->setColor(color);
	draw();
}


boolean Press_t::getPressEvent(PRESS_TYPE press)
{
	refresh();
	if(pressEvent == press) {
		reqClear = true;
		return true;
	} else
		return false;
}

void Press_t::refresh() {
	if (pTouch->dataAvailable() == true) {
		prevPressTime = millis() + PRESS_DELAY_TIME_MS;
		if(prevPressState == RELEASED) {
			pressEvent = PRESSED;
		} else if(reqClear) {
			reqClear = false;
			pressEvent = NOT_PRESS;
		}
		prevPressState = PRESSED;
	} else if(millis() > prevPressTime || pressEvent == PRESSED) {
		if(prevPressState == PRESSED && millis() > prevPressTime) {
			prevPressState = RELEASED;
			pressEvent = RELEASED;
		} else if (reqClear) {
			reqClear = false;
			pressEvent = NOT_PRESS;
		}
	}
}
