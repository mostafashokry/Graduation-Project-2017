/*
 * PSD_Man.h
 *
 * Created: 26/02/2017 08:27:03 م
 *  Author: Shawkat
 */ 


#ifndef PSD_MAN_H_
#define PSD_MAN_H_

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <util/delay.h>



void psd_init(void);
void psd_clear(void);
unsigned char psd_read (void);





#endif /* PSD-MAN_H_ */