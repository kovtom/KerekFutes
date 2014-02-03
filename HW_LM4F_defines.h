#ifndef _HW_LM4F_DEFINES_H_
#define _HW_LM4F_DEFINES_H_

// *** LM4F120H5QR hardverspecifikus funkciok ***
#define portOutputRegister(x)	(regtype)portBASERegister(x)
#define portInputRegister(x)	(regtype)portBASERegister(x)

#define cbi(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi(reg, mask) GPIOPinWrite(reg, mask, mask)
#define rbi(reg, mask) GPIOPinRead(reg, mask)

#define pulse_high(reg, bitmask) { sbi(reg, bitmask); cbi(reg, bitmask); }
#define pulse_low(reg, bitmask) { cbi(reg, bitmask); sbi(reg, bitmask); }

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) cfont.font[x]  

#define pgm_read_word(data) *(data)
#define pgm_read_byte(data) *(data)
#define PROGMEM
typedef volatile uint32_t regtype;
typedef uint8_t regsize;
typedef unsigned short* bitmapdatatype;

#define TONE_PIN PD_0

#endif //_HW_LM4F_DEFINES_H_