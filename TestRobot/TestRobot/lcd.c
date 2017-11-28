/*
 * lcd.c
 *
 * Created: 2/14/2017 12:07:33 AM
 *  Author: Mahmoud
 */ 

#include "lcd.h"



void lcd_cmd(unsigned char cmd)
{
	LCD_CTR_EN = 0x00;	//E=0 , RS=0
	LCD_DATA_EN=cmd;
	LCD_CTR_EN = 0x04;	//E=1 , RS=0
	asm volatile(" NOP "); // delay for about 125 ns in 8MHZ
	asm volatile(" NOP "); // delay for about 125 ns in 8MHZ
	LCD_CTR_EN = 0x00;	//E=0 , RS=0
	_delay_us(50);
}

void lcd_data(unsigned char data)
{
	LCD_CTR_EN = 0x01;	//E=0 , RS=1
	LCD_DATA_EN=data;
	LCD_CTR_EN = 0x05;	//E=1 , RS=1
	asm volatile(" NOP "); // delay for about 125 ns in 8MHZ
	asm volatile(" NOP "); // delay for about 125 ns in 8MHZ
	LCD_CTR_EN = 0x01;	//E=0 , RS=1
	_delay_us(50);
}

void lcd_cls(void)
{
	lcd_cmd(0x01); // Delete LCD Screen
	lcd_cmd(0x80);
	_delay_ms(2);
}

void LcdClear(void)
{
	lcd_cmd(0x01); // Delete LCD Screen
}

void LcdInit(void)
{
	LCD_CTR_EN = 0x00;	//E=0 , RS=0
	_delay_ms(2);
	
	lcd_cmd(0x38);	 // Set LCD Function ( DL -> B bits, N ~> 2 lines)
	lcd_cmd(0x0C);  // Set LCD Screen ( D -> display on, C -> cursor off, B -> blink off)
	lcd_cmd(0x01); // Delete LCD Screen
	_delay_ms(2);
}

void lcd_setpos(unsigned char pos)
{
	lcd_cmd(pos);	// start position of string
}

void LcdString(unsigned char pos, char *str)
{
	// Set an external memory
	MCUCR = 0x80;
	XMCRA = 0x44;
	XMCRB = 0x80;
	
	LcdInit();
	
	lcd_cmd(pos);
	
	while(*str != '\0')		// display string
	{
		if ((*str >= 0X20) && (*str <= 0x7F))
		{
			lcd_data((unsigned char)*str);
		}
		str++;
	}
}