/*
 * SPI.h
 *
 * Created: 09/03/2017 02:47:35 م
 *  Author: Shawkat
 */ 


#ifndef SPI_H_
#define SPI_H_
/////////////////////////////////////////////////
///////////// Macros ///////////////////////////
////////////////////////////////////////////////

#define SS		0
#define SCK		1
#define MOSI	2
#define MISO	3

#define SPSR_INIT		0x00			//0b00XX XXX0
#define SPCR_M_INIT		0xF6			//0b 1111 1110 the LSB of the data word is transmitted first, cpol 1 ,cpha1
#define SPCR_S_INIT		0xC6			//0b 1110 1110

#define MASTER	1
#define SLAVE	0
//////////////////////////////////////////////////
/////////////// Prototypes //////////////////////
/////////////////////////////////////////////////

void init_SPI (char type);
void trancieve_SPI (char data);
void (*SPI_ptr)(void);
void Set_SPI_Recieve (void (*ptr)(void));

#endif /* SPI_H_ */