/*
 * Ext_Memo.c
 *
 * Created: 31/01/2017 05:52:57 م
 *  Author: Shawkat
 */ 
#include <avr/io.h>
#include "Ext_Memory.h"



void Ext_memo_init(void)
{
	// Set TIMER No. 3 to 1ms cycle
	TCCR3B = 0x0c;                        // Clock Select, clk/256 (From prescaler)
	OCR3A = 0x1e;                        // (F_CPU / 256 / 1000) - 1;
	TCNT3 = 0x0000;                        // clear Timer/Counter3

	// TIMER No. 3 interrupt ENABLE
// 	ETIMSK = (1 << OCIE3A);                // enable OC3A interrupt
// 
// 	XMCRA = XMCRA_INIT ;
// 	XMCRB = XMCRB_INIT ;
// 	MCUCR = MCUCR_INIT ;
}
