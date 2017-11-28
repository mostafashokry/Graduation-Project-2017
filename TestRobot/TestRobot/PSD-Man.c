/*
 * PSD_Man.c
 *
 * Created: 26/02/2017 07:37:16 م
 *  Author: Shawkat
 */ 

#include "PSD_Man.h"

//from Kinematics
extern double current_position[3] ;  
double PoseAtInt[3];

volatile unsigned int psd[3];
void psd_init(void)
{
	// Set a communication speed to 38400.
	UBRR1H = 0;  // BAUD Rate Hi
	UBRR1L = 12; // BAUD Rate Low(F_cpu / 16UL * 38400) - 1)
	// RX Interrupt Enable. Rx / Tx Enable
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	UCSR1C = (1<<UCSZ1) | (1<<UCSZ0); // no parity, 1 stop. 8 data
	
	psd[0] = 0;
	psd[1] = 0;
	psd[2] = 0;
}


ISR (USART1_RX_vect)
{
	static unsigned char chk=0 ,rxcnt=0;
	static unsigned char rxdata[7];
	unsigned char rx,status;

	status = UCSR1A;
	rx = UDR1;
	
	if (status &((1<<FE)|(1<<DOR))) //1
	{
		
		rxcnt=0;
	}
	else //1
	{
		if (rxcnt==0) //2
		{
			if (rx == 0xCC) //3
			{
				rxdata[rxcnt]=rx;
				rxcnt++;
				chk = rx;
			}
		}
		else//2
		{
			if (rxcnt==0)//4
			{
				if (rx == 0xCC) //5
				{
					rxdata[rxcnt]=rx;
					rxcnt++;
					chk = rx;
				}
			}
			else //4
			{
				rxdata[rxcnt]=rx;
				if (rxcnt !=5) //6
				{
					chk+=rx;
				}
				rxcnt++;
				if (rxcnt == 7) //7
				{
					if (chk == rxdata[5]) //8
					{
						psd[rxdata[1] - 0x11]=(unsigned int)rxdata[3]<<8 | (unsigned int)rxdata[4];
					}
					
					rxcnt=0;
				}
				
			}
		}
	}
}

void psd_clear(void)
{
	psd[0] = 0;
	psd[1] = 0;
	psd[2] = 0;

}


unsigned char psd_read (void)
{
	unsigned char id;
	unsigned int timeout, i;
	unsigned char data[7];
	
	data[0]=0xAA;
	
	data[2]=0x07;
	data[3]=0x00;
	data[4]=0x00;
	
	data[6]=0x55;


	for (id = 0 ;id<3;id++)
	{
		data[1]=0x11+id;
		data[5]=data[0]+data[1]+data[2]+data[3]+data[4]+data[6];
		
		for (i=0;i<7;i++)
		{
			timeout=4000;
			while(!(UCSR1A & (1<<UDRE)) &&timeout--);
			{
				if (timeout == 0)
				{
					break;
				}
			}
			UDR1 = data[i];
		}
	}
	
	if (psd[0]>0 && psd[1]>0 && psd[2]>0)
	{
// 		PoseAtInt[0]=current_position[0];
// 		PoseAtInt[1]=current_position[1];
// 		PoseAtInt[2]=current_position[2];
// 		
		return 1;
	}
	return 0;
}
