/*
 * Ext_Memory.h
 *
 * Created: 31/01/2017 05:52:31 م
 *  Author: Shawkat
 */ 


#ifndef EXT_MEMORY_H_
#define EXT_MEMORY_H_ 
#define MCUCR_INIT 0x80			//0b10xx xxxx
#define XMCRA_INIT 0x44			//0bx000 000x	
#define XMCRB_INIT 0x80			//0b1xxx x000

void Ext_memo_init(void);

#endif /* EXT_MEMORY_H_ */