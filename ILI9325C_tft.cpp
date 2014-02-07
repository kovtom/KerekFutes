// 
// 
// 

#include "ILI9325C_tft.h"

/*
  You can always find the latest version of the library at 
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would 
  appreciate that you share the code with me so that I might include 
  it in the next release. I can be contacted through 
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/

#include "ILI9325C_tft.h"
#include <pins_energia.h>

#pragma message("Compiling for Stellarpad (LM4F120H5QR)...")

ILI9325C_tft::ILI9325C_tft()
{
}

ILI9325C_tft::ILI9325C_tft(byte model, int RS_, int WR_, int CS_, int RST_, int SER_)
{
	// Init ILI9325C
	disp_x_size=239;
	disp_y_size=319;
	display_transfer_mode=8;

	display_model=model;
	CS = CS_;
	RS = RS_;
	WR = WR_;
	RST = RST_;
	SER = SER_;
}

void ILI9325C_tft::initPins()
{
	_set_direction_registers(display_transfer_mode);
	P_RS	= portOutputRegister(digitalPinToPort(RS));
	B_RS	= digitalPinToBitMask(RS);
	P_WR	= portOutputRegister(digitalPinToPort(WR));
	B_WR	= digitalPinToBitMask(WR);
	P_CS	= portOutputRegister(digitalPinToPort(CS));
	B_CS	= digitalPinToBitMask(CS);
	P_RST	= portOutputRegister(digitalPinToPort(RST));
	B_RST	= digitalPinToBitMask(RST);

	pinMode(RS,OUTPUT);
	pinMode(WR,OUTPUT);
	pinMode(CS,OUTPUT);
	pinMode(RST,OUTPUT);
}

void ILI9325C_tft::LCD_Write_COM(char VL)  
{   
	cbi(P_RS, B_RS);
	LCD_Writ_Bus(0x00,VL,display_transfer_mode);
}

void ILI9325C_tft::LCD_Write_DATA(char VH,char VL)
{
	sbi(P_RS, B_RS);
	LCD_Writ_Bus(VH,VL,display_transfer_mode);
}

void ILI9325C_tft::LCD_Write_DATA(char VL)
{
	sbi(P_RS, B_RS);
	LCD_Writ_Bus(0x00,VL,display_transfer_mode);
}

void ILI9325C_tft::LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

void ILI9325C_tft::InitLCD(byte orientation)
{
	// moved from ctor
	initPins();
	orient=orientation;
	_hw_special_init();

	sbi(P_RST, B_RST);
	delay(5); 
	cbi(P_RST, B_RST);
	delay(15);
	sbi(P_RST, B_RST);
	delay(15);

	cbi(P_CS, B_CS);

	LCD_Write_COM_DATA(0xE5, 0x78F0); // set SRAM internal timing
	LCD_Write_COM_DATA(0x01, 0x0100); // set Driver Output Control  
	LCD_Write_COM_DATA(0x02, 0x0700); // set 1 line inversion  
	LCD_Write_COM_DATA(0x03, 0x1030); // set GRAM write direction and BGR=1.  
	LCD_Write_COM_DATA(0x04, 0x0000); // Resize register  
	LCD_Write_COM_DATA(0x08, 0x0207); // set the back porch and front porch  
	LCD_Write_COM_DATA(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]  
	LCD_Write_COM_DATA(0x0A, 0x0000); // FMARK function  
	LCD_Write_COM_DATA(0x0C, 0x0000); // RGB interface setting  
	LCD_Write_COM_DATA(0x0D, 0x0000); // Frame marker Position  
	LCD_Write_COM_DATA(0x0F, 0x0000); // RGB interface polarity  
	//*************Power On sequence ****************//  
	LCD_Write_COM_DATA(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB  
	LCD_Write_COM_DATA(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]  
	LCD_Write_COM_DATA(0x12, 0x0000); // VREG1OUT voltage  
	LCD_Write_COM_DATA(0x13, 0x0000); // VDV[4:0] for VCOM amplitude  
	LCD_Write_COM_DATA(0x07, 0x0001);  
	delay(200); // Dis-charge capacitor power voltage  
	LCD_Write_COM_DATA(0x10, 0x1090); // SAP, BT[3:0], AP, DSTB, SLP, STB  
	LCD_Write_COM_DATA(0x11, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]  
	delay(50); // Delay 50ms  
	LCD_Write_COM_DATA(0x12, 0x001F); // 0012  
	delay(50); // Delay 50ms  
	LCD_Write_COM_DATA(0x13, 0x1500); // VDV[4:0] for VCOM amplitude  
	LCD_Write_COM_DATA(0x29, 0x0027); // 04  VCM[5:0] for VCOMH  
	LCD_Write_COM_DATA(0x2B, 0x000D); // Set Frame Rate  
	delay(50); // Delay 50ms  
	LCD_Write_COM_DATA(0x20, 0x0000); // GRAM horizontal Address  
	LCD_Write_COM_DATA(0x21, 0x0000); // GRAM Vertical Address  
	// ----------- Adjust the Gamma Curve ----------//  
	LCD_Write_COM_DATA(0x30, 0x0000);  
	LCD_Write_COM_DATA(0x31, 0x0707);  
	LCD_Write_COM_DATA(0x32, 0x0307);  
	LCD_Write_COM_DATA(0x35, 0x0200);  
	LCD_Write_COM_DATA(0x36, 0x0008);  
	LCD_Write_COM_DATA(0x37, 0x0004);  
	LCD_Write_COM_DATA(0x38, 0x0000);  
	LCD_Write_COM_DATA(0x39, 0x0707);  
	LCD_Write_COM_DATA(0x3C, 0x0002);  
	LCD_Write_COM_DATA(0x3D, 0x1D04);  
	//------------------ Set GRAM area ---------------//  
	LCD_Write_COM_DATA(0x50, 0x0000); // Horizontal GRAM Start Address  
	LCD_Write_COM_DATA(0x51, 0x00EF); // Horizontal GRAM End Address  
	LCD_Write_COM_DATA(0x52, 0x0000); // Vertical GRAM Start Address  
	LCD_Write_COM_DATA(0x53, 0x013F); // Vertical GRAM Start Address  
	LCD_Write_COM_DATA(0x60, 0xA700); // Gate Scan Line  
	LCD_Write_COM_DATA(0x61, 0x0001); // NDL,VLE, REV   
	LCD_Write_COM_DATA(0x6A, 0x0000); // set scrolling line  
	//-------------- Partial Display Control ---------//  
	LCD_Write_COM_DATA(0x80, 0x0000);  
	LCD_Write_COM_DATA(0x81, 0x0000);  
	LCD_Write_COM_DATA(0x82, 0x0000);  
	LCD_Write_COM_DATA(0x83, 0x0000);  
	LCD_Write_COM_DATA(0x84, 0x0000);  
	LCD_Write_COM_DATA(0x85, 0x0000);  
	//-------------- Panel Control -------------------//  
	LCD_Write_COM_DATA(0x90, 0x0010);  
	LCD_Write_COM_DATA(0x92, 0x0600);  
	LCD_Write_COM_DATA(0x07, 0x0133); // 262K color and display ON        

	sbi (P_CS, B_CS); 

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font=0;
	_transparent = false;
}

void ILI9325C_tft::setXY(word x1, word y1, word x2, word y2)
{
	if (orient==LANDSCAPE)
	{
		swap(word, x1, y1);
		swap(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(word, y1, y2)
	}

	LCD_Write_COM_DATA(0x20,x1);
	LCD_Write_COM_DATA(0x21,y1);
	LCD_Write_COM_DATA(0x50,x1);
	LCD_Write_COM_DATA(0x52,y1);
	LCD_Write_COM_DATA(0x51,x2);
	LCD_Write_COM_DATA(0x53,y2);
	LCD_Write_COM(0x22); 
}

void ILI9325C_tft::clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void ILI9325C_tft::drawRectBold(int x1, int y1, int x2, int y2, int width)
{
	for(int i = 0; i < width; i++)
	{
		drawRect(x1+i,y1+i,x2-i,y2-i);
	}
}


void ILI9325C_tft::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void ILI9325C_tft::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void ILI9325C_tft::fillRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	//Nem kell
//	if (display_transfer_mode==16)
//	{
//		cbi(P_CS, B_CS);
//		setXY(x1, y1, x2, y2);
//		sbi(P_RS, B_RS);
//		_fast_fill_16(fch,fcl,((long(x2-x1)+1)*(long(y2-y1)+1)));
//		sbi(P_CS, B_CS);
//	}
//	else if ((display_transfer_mode==8) and (fch==fcl))
	if ((display_transfer_mode==8) and (fch==fcl))
	{
		cbi(P_CS, B_CS);
		setXY(x1, y1, x2, y2);
		sbi(P_RS, B_RS);
		_fast_fill_8(fch,((long(x2-x1)+1)*(long(y2-y1)+1)));
		sbi(P_CS, B_CS);
	}
	else
	{
		if (orient==PORTRAIT)
		{
			for (int i=0; i<((y2-y1)/2)+1; i++)
			{
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
		else
		{
			for (int i=0; i<((x2-x1)/2)+1; i++)
			{
				drawVLine(x1+i, y1, y2-y1);
				drawVLine(x2-i, y1, y2-y1);
			}
		}
	}
}

void ILI9325C_tft::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void ILI9325C_tft::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	cbi(P_CS, B_CS);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(fch,fcl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(fch,fcl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(fch,fcl);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(fch,fcl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(fch,fcl);
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void ILI9325C_tft::fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void ILI9325C_tft::clrScr()
{
	//Nem hasznalt
	//long i;
	
	cbi(P_CS, B_CS);
	clrXY();
	sbi(P_RS, B_RS);
	_fast_fill_8(0,((disp_x_size+1)*(disp_y_size+1)));
	sbi(P_CS, B_CS);
}

void ILI9325C_tft::fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void ILI9325C_tft::fillScr(word color)
{
	long i;
	char ch, cl;
	
	ch=byte(color>>8);
	cl=byte(color & 0xFF);

	cbi(P_CS, B_CS);
	clrXY();
	sbi(P_RS, B_RS);
	if ((display_transfer_mode==8) and (ch==cl))
		_fast_fill_8(ch,((disp_x_size+1)*(disp_y_size+1)));
	else
	{
		for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
		{
			//Mivel a alabbi else nem fut le az if sem kell
//			if (display_transfer_mode!=1)
				LCD_Writ_Bus(ch,cl,display_transfer_mode);
			//Ez elvileg nem fut le
//			else
//			{
//				LCD_Writ_Bus(1,ch,display_transfer_mode);
//				LCD_Writ_Bus(1,cl,display_transfer_mode);
//			}
		}
	}
	sbi(P_CS, B_CS);
}

void ILI9325C_tft::setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void ILI9325C_tft::setColor(word color)
{
	fch=byte(color>>8);
	fcl=byte(color & 0xFF);
}

word ILI9325C_tft::getColor()
{
	return (fch<<8) | fcl;
}

void ILI9325C_tft::setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void ILI9325C_tft::setBackColor(uint32_t color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=byte(color>>8);
		bcl=byte(color & 0xFF);
		_transparent=false;
	}
}

word ILI9325C_tft::getBackColor()
{
	return (bch<<8) | bcl;
}

void ILI9325C_tft::setPixel(word color)
{
	LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void ILI9325C_tft::drawPixel(int x, int y)
{
	cbi(P_CS, B_CS);
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	sbi(P_CS, B_CS);
	clrXY();
}

void ILI9325C_tft::drawLineBold(int x1, int y1, int x2, int y2, int width)
{
	
	if(x1==x2 || y1==y2)
	{
		for(int i = 0; i < width; i++)
		{
			if(x1==x2)
				drawLine(x1+i,y1,x2+i,y2);
			else
				drawLine(x1,y1+i,x2,y2+i);
		}
	} else {
		drawPixel(x1,y1);
	}
}

void ILI9325C_tft::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		cbi(P_CS, B_CS);
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
		sbi(P_CS, B_CS);
	}
	clrXY();
}

void ILI9325C_tft::drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	cbi(P_CS, B_CS);
	setXY(x, y, x+l, y);

	if ((display_transfer_mode==8) and (fch==fcl))
	{
		sbi(P_RS, B_RS);
		_fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void ILI9325C_tft::drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	cbi(P_CS, B_CS);
	setXY(x, y, x, y+l);
	if ((display_transfer_mode==8) and (fch==fcl))
	{
		sbi(P_RS, B_RS);
		_fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	clrXY();
}

void ILI9325C_tft::printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 

	cbi(P_CS, B_CS);
  
	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
				temp++;
			}
		}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							setPixel((fch<<8)|fcl);
						} 
						else
						{
							setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++) 
		{
			for (int zz=0; zz<(cfont.x_size/8); zz++)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++)
				{   
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
	 
					if((ch&(1<<(7-i)))!=0)   
					{
       						setPixel((fch<<8)|fcl);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}

	sbi(P_CS, B_CS);
	clrXY();
}

void ILI9325C_tft::rotateChar(byte c, int x, int y, int pos, int deg)
{
#ifndef DISABLE_FLOATING_POINT
	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian=deg*0.0175;  

	cbi(P_CS, B_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	sbi(P_CS, B_CS);
	clrXY();
#endif
}

void ILI9325C_tft::print(const char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
#ifdef DISABLE_FLOATING_POINT
		printChar(*st++, x + (i*(cfont.x_size)), y);
#else
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
#endif
}

void ILI9325C_tft::print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void ILI9325C_tft::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

#ifndef DISABLE_FLOATING_POINT
void ILI9325C_tft::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (unsigned int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (unsigned int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (unsigned int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st,x,y);
}
#endif
/*
void ILI9325C_tft::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}
*/
void ILI9325C_tft::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* ILI9325C_tft::getFont()
{
	return cfont.font;
}

uint8_t ILI9325C_tft::getFontXsize()
{
	return cfont.x_size;
}

uint8_t ILI9325C_tft::getFontYsize()
{
	return cfont.y_size;
}

void ILI9325C_tft::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			cbi(P_CS, B_CS);
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
			sbi(P_CS, B_CS);
		}
		else
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
					LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
			sbi(P_CS, B_CS);
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
			}
			sbi(P_CS, B_CS);
		}
		else
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
			sbi(P_CS, B_CS);
		}
	}
	clrXY();
}

void ILI9325C_tft::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
#ifndef DISABLE_FLOATING_POINT
	else
	{
		unsigned int col;
		int tx, ty, newx, newy;
		double radian=deg*0.0175;  

		cbi(P_CS, B_CS);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
				LCD_Write_DATA(col>>8,col & 0xff);
			}
		sbi(P_CS, B_CS);
	}
#endif
	clrXY();
}

void ILI9325C_tft::lcdOff()
{
	cbi(P_CS, B_CS);
	sbi(P_CS, B_CS);
}

void ILI9325C_tft::lcdOn()
{
	cbi(P_CS, B_CS);
	sbi(P_CS, B_CS);
}

void ILI9325C_tft::setContrast(char c)
{
	cbi(P_CS, B_CS);
	sbi(P_CS, B_CS);
}

int ILI9325C_tft::getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int ILI9325C_tft::getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

void ILI9325C_tft::_hw_special_init()
{
	sbi(P_CS, B_CS);
	sbi(P_RS, B_RS);
	sbi(P_WR, B_WR);
}

/*
inline void write_byte_lo(unsigned char ch)
{
	ROM_GPIOPinWrite(GPIO_PORTD_BASE, 0x0f, ch);
	ROM_GPIOPinWrite(GPIO_PORTA_BASE, 0xf0, ch);
}
*/
inline void write_byte_hi(unsigned char ch)
{
	ROM_GPIOPinWrite(GPIO_PORTB_BASE, 0xff, ch);
}

void ILI9325C_tft::LCD_Writ_Bus(char VH,char VL, byte mode)
{   
	//Nem kell
//	switch (mode)
//	{
//	case 8:
		write_byte_hi(VH);
		pulse_low(P_WR, B_WR);
		write_byte_hi(VL);
		pulse_low(P_WR, B_WR);
//		break;
//	}
}

void ILI9325C_tft::_set_direction_registers(byte mode)
{
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xff);
}

//void ILI9325C_tft::_fast_fill_16(int ch, int cl, long pix){}

/*
void ILI9325C_tft::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

	write_byte_lo(cl);
	write_byte_hi(ch);

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);
		}
}
*/

void ILI9325C_tft::_fast_fill_8(int ch, long pix)
{
	long blocks;
	write_byte_hi(ch);

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}

void ILI9325C_tft::_convert_float(char *buf, double num, int width, byte prec)
{
	//	NYI
}

ILI9325C_tft ILI9325C_TFT;

