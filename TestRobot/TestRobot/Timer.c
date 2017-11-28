/*
 * Timer.c
 *
 * Created: 25/03/2017 08:59:24 م
 *  Author: Shawkat
 */ 
#include <avr/interrupt.h>
#include "Timer.h"


volatile char Timer_counter = 0;
void Timer_init (void)
{
	TCCR0 = TCCR0_INIT;
	TIMSK = TIMSK_INIT;
	TCNT0 = 0x00;
}


void Timer_Reset (void)
{
	Timer_counter = 0;
	TCNT0 = 0;
}

ISR(TIMER0_OVF_vect)
{
	Timer_counter++;
}