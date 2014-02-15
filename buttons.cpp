#include "buttons.h"

//Box_t constructor
Box_t::Box_t(	ILI9325C_tft * screenP_,
				int x_,
				int y_,
				int w_,
				int h_,
				int think_,
				int primFrameColor_,
				int secFrameColor_,
				int normBackgroundColor_,
				int invBackgroundColor_,
				int textColor_,
				uint8_t * font_,
				char * text_,
				int tx_,
				int ty_,
				uint8_t priority_
			) : Point_t(x_, y_)
{
	pScreen = screenP_;
	width = w_;
	height = h_;
	think = think_;
	primFrameColor = primFrameColor_;
	secFrameColor = secFrameColor_;
	normBackgroundColor = normBackgroundColor_;
	invBackgroundColor = invBackgroundColor_;

	textColor = textColor_;
	font = font_;
	
	textLength = _textLength(text_);
	text = new char[ textLength + 1 ];
	_copyText(text_);

	tx = tx_;
	ty = ty_;
	
	priority = priority_;
	status = NORMAL;
}

void Box_t::_copyText(char * inpText) {
	uint8_t idx = 0;
	while(*inpText) {
		text[idx] = *inpText;
		idx++; inpText++;
	}
	text[idx] = 0x00;
}

uint8_t Box_t::_textLength(char * inpText) {
	uint8_t cnt = 0;
	while(*inpText) {
		cnt++;
		inpText++;
	}
	return (cnt);
}

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
	status = NORMAL;
	draw( getPrimFrameColor() );
	pScreen->setColor( getNormBackgroundColor() );
	pScreen->fillRect(getX() + think, getY() + think,
		getX() + width - think, getY() + height -think);

	if(font) {
		pScreen->setBackColor( getNormBackgroundColor() );
		print();
	}
}

//kirajzol egy aktiv dobozt
void Box_t::drawInvert()
{
	status = INVERT;
	draw( getSecFrameColor() );
	pScreen->setColor( getInvBackgroundColor() );
	pScreen->fillRect(getX() + think, getY() + think,
		getX() + width - think, getY() + height -think);

	if(font) {
		pScreen->setBackColor( getInvBackgroundColor() );
		print();
	}
}

//Beallit egy uj hatterszint
void Box_t::setNormBackgroundColor(int color) {
	normBackgroundColor = color;
	//Ha eppen ez a status az aktualis akkor ki is rajzoljuk
	//if(status == NORMAL)
	//	drawNormal();
}

//Beallit egy uj Inverz hatterszint
void Box_t::setInvBackgroundColor(int color) {
	invBackgroundColor = color;
	//if(status == INVERT)
	//	drawInvert();
}



//Az elso feliratot beallitja
void Box_t::_setNewText(char * inpText) {
	textLength = _textLength(inpText);
	text = new char[ textLength + 1 ];
	_copyText(inpText);
}

//Regi felirat torlese
void Box_t::_clearOldTExt(boolean clear) {
	//Letoroljuk a regi feliratot, ha clear true
	if(clear) {
		for(uint8_t i = 0; i < textLength; i++)
			text[i] = ' ';
		print();
	}
	delete [] text;
}

//Uj feliratot allit be de a clear alapjan torli/vagy sem az elozot
void Box_t::setText(char * inpText, boolean clear) {
	//Letoroljuk a regi feliratot
	_clearOldTExt(clear);
	//Beallitjuk az uj feliratot es ki is irjuk
	_setNewText(inpText);
	print();
}

//Uj szamfeliratot allit be a hozzakapcsolt sztringgel
void Box_t::setText(int number, char * inpText, boolean clear) {
	//Letoroljuk a regi feliratot
	_clearOldTExt(clear);
	//A szamot string-ge alakitjuk
	char s[10];
	itoa(number, s, 10);
	
	//Meg kell nezni, hogy az inpText milyen hosszu
	uint8_t inpTextLength = _textLength(inpText);
	uint8_t sLength = _textLength(s);

	char * tmpS;
	tmpS = new char [ sLength + inpTextLength + 1 ];
	char * ptmpS = tmpS;
	char * ps = s;

	//A ket string osszemasolasa
	uint8_t cnt = 0;
	while(*ps) {
		ptmpS[cnt] = *ps;
		cnt++; ps++;
	}
	while(*inpText) {
		ptmpS[cnt] = *inpText;
		cnt++; inpText++;
	}
	ptmpS[cnt] = 0;
	_setNewText(ptmpS);
	delete [] tmpS;
	print();
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
void Box_t::print()
{
	if(status == NORMAL) 
		pScreen->setBackColor(normBackgroundColor);
	else
		pScreen->setBackColor(invBackgroundColor);
	pScreen->setColor(textColor);
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

//Press_t inicializalasa a default constructor miatt
void Press_t::begin(UTouch * pTouch_)
{
	pTouch = pTouch_;
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
		} else {
			return OTHER_RELEASED;
		}
	} else {
		return NOT_PRESS;
	}
}