/*
 * key.c
 *
 * Created: 2/13/2017 11:51:56 PM
 *  Author: Mahmoud
 */ 

#include "key.h"

unsigned char KeyIn(void)
{
	// Set an external memory
	MCUCR = 0x80;
	XMCRA = 0x44;
	XMCRB = 0x80;
	
	// KEY uses the lower 4bit (0~3) only.
	// Disuse an upper bit through AND operation for 0x0F and return it
	return(KEY_IN_EN & 0x0F);
}