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

//Kirajzol egy inaktiv doboz hatterrel
void Box_t::drawNormal()
{
	draw( getPrimFrameColor() );
	pScreen->setColor( getNormBackgroundColor() );
	pScreen->fillRect(getX() + think, getY() + think,
		getX() + width - think, getY() + height -think);

	if(font) {
		pScreen->setBackColor( getNormBackgroundColor() );
		printText();
	}
}

//kirajzol egy aktiv dobozt
void Box_t::drawInvert()
{
	draw( getSecFrameColor() );
	pScreen->setColor( getInvBackgroundColor() );
	pScreen->fillRect(getX() + think, getY() + think,
		getX() + width - think, getY() + height -think);

	if(font) {
		pScreen->setBackColor( getInvBackgroundColor() );
		printText();
	}
}

//Megnezi, hogy a megadott koordinataju pont benne van-e a dobozban
boolean Box_t::pointInBox(int x, int y)
{
	int xMin = getX();
	int yMin = getY();
	int xMax = xMin + width;
	int yMax = yMin + height;
	if(x >= xMin && x <= xMax && y >= yMin && y <= yMax)
		return true;
	else
		return false;
}

//Kiirja a boxba a text-et
void Box_t::printText()
{
	pScreen->setColor(VGA_WHITE);
	pScreen->setFont(font);

	int xPrint, yPrint;
	int xCenter = getBoxX() + (width / 2);
	int yCenter = getBoxY() + (height / 2);
	int xTextWidth = pScreen->getFontXsize() * textLength; 
	int yTextWidth = pScreen->getFontYsize();
	
	if(tx == CENTER)
		xPrint = xCenter - xTextWidth / 2;
	else 
		xPrint = getBoxX() + tx;

	if(ty == CENTER)
		yPrint = yCenter - yTextWidth / 2;
	else
		yPrint = getBoxY() + ty;
	pScreen->print(text, xPrint, yPrint);
}

//Megvizsgalja, hogy a parameterkent kapott pressEvent bekovetkezett-e?
boolean Press_t::getPressEvent(PRESS_TYPE press)
{
	refreshTouchEvent();
	if(pressEvent == press) {
		reqClear = true;
		return true;
	} else
		return false;
}

//Frissiti a Press_t osztaly valtozoit
//Minimum PRESS_DELAY_TIME_MS ideig nem erzekel uj esemenyt az screen
//elengedese utan
void Press_t::refreshTouchEvent() {
	if (pTouch->dataAvailable() == true) {
		pTouch->read();
		int x = pTouch->getX();
		int y = pTouch->getY();
		if(x > 0 && y > 0) {
			setX(x);
			setY(y);
		}
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

PRESS_TYPE Button_t::getButtonEvent()
{
	if( getPressEvent(PRESSED) ) {
		if( pointInBox( getPressX(), getPressY() ) ) {
			tone(TONE_PIN, 1000, 50);
			drawInvert();
			prevState = PRESSED;
			return PRESSED;
		}
	}
	else if( getPressEvent(RELEASED) && prevState == PRESSED ) {
		prevState = NOT_PRESS;
		drawNormal();
		int x = getPressX(); int y = getPressY();
		if( pointInBox( getPressX(), getPressY() ) ) {
			return RELEASED;
		}
	} else {
		return NOT_PRESS;
	}
}