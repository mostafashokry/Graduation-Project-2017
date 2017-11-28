/*
 * key.h
 *
 * Created: 2/13/2017 11:47:17 PM
 *  Author: Mahmoud
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#ifndef KEY_H_
#define KEY_H_

// External Memory Address Define
#define KEY_IN_EN			(*(volatile unsigned char *)0x2200)

//Key
#define KEY0	0x0E
#define KEY1	0x0D
#define KEY2	0x0B
#define KEY3	0x07

#endif /* KEY_H_ */

///////////////////////////////////////////////////////
//////////////// Prototypes ///////////////////////////
///////////////////////////////////////////////////////

unsigned char KeyIn(void);