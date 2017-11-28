/*
 * led.c
 *
 * Created: 2/13/2017 11:38:36 PM
 *  Author: Mahmoud
 */ 

#include "led.h"

void LedOn(unsigned char led)
{
	
	// Set an external memory
	MCUCR = 0x80;
	XMCRA = 0x44;
	XMCRB = 0x80;
	
	// Turn on the corresponding LED
	// The upper 3bit is LED 0110 and the lower 4bit (0~3) is KEY PULLUP
	
	LED_OUT_EN = ~(led);
}

