/*
 * SPI.c
 *
 * Created: 09/03/2017 02:47:20 م
 *  Author: Shawkat
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "Common.h"


extern char Path [100][2];

void init_SPI (char type)
{
	SEI();
	switch(type)
	{
		case MASTER:
			SPCR = SPCR_M_INIT;
			SPSR = SPSR_INIT;
			
			Set_Pin(DDRB,SS);
			Set_Pin(DDRB,SCK);
			Set_Pin(DDRB,MOSI);
			Clear_Pin(DDRB,MISO);
			break;
			
		case SLAVE:
			SPCR = SPCR_S_INIT;
			SPSR = SPSR_INIT;
		
			Clear_Pin(DDRB,SS);
			Clear_Pin(DDRB,SCK);
			Clear_Pin(DDRB,MOSI);
			Set_Pin(DDRB,MISO);
			break;
	}
}
void trancieve_SPI (char data)
{
	SPDR= data;
}
void (*SPI_ptr)(void);
void Set_SPI_Recieve (void (*ptr)(void))
{
	SPI_ptr=ptr;
}
char recieved_data ;
ISR(SPI_STC_vect)
{
	SPI_ptr();
}