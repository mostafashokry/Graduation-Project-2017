/*
 * lcd.h
 *
 * Created: 2/14/2017 12:00:19 AM
 *  Author: Mahmoud
 */ 


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#ifndef LCD_H_
#define LCD_H_

// External Memory Address Define
#define LCD_DATA_EN		(*(volatile unsigned char *)0x2000)
#define LCD_CTR_EN		(*(volatile unsigned char *)0x2100)

// LCD cursor position value
#define LCD_LINE1	0x80
#define LCD_LINE2	0xC0


///////////////////////////////////////////////////////
//////////////// Prototypes ///////////////////////////
///////////////////////////////////////////////////////

void lcd_cmd(unsigned char cmd);

void lcd_data(unsigned char data);

void lcd_cls(void);

void LcdClear(void);

void LcdInit(void);

void lcd_setpos(unsigned char pos);

void LcdString(unsigned char pos, char *str);
#endif /* LCD_H_ */
