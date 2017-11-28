/*
 * led.h
 *
 * Created: 2/13/2017 11:32:21 PM
 *  Author: Mahmoud
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#ifndef LED_H_
#define LED_H_

// External Memory Address Define
#define LED_OUT_EN			(*(volatile unsigned char *)0x2300) 

// Operate LED on the front panel

#define RLED 0x10             // Red LED (PW) -> 0xEF ( 1110 1111 )
#define YLED 0x20	     	 // Yellow LEO (ST) -> 0xDF ( 1101 1111 )
#define GLED 0x40			// Green LED (AL) -> 0xBF ( 1011 1111 )


#endif /* LED_H_ */

///////////////////////////////////////////////////////
//////////////// Prototypes ///////////////////////////
///////////////////////////////////////////////////////

void LedOn(unsigned char led);