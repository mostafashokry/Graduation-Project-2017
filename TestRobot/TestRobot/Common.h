/*
 * Common.h
 *
 * Created: 10/09/2016 01:13:38 م
 *  Author: Shawkat
 */ 



#ifndef COMMON _H_
#define COMMON _H_
#define Clear_Pin(reg , Pin_Num) reg= (reg & (~(1<<Pin_Num)))
#define Set_Pin(reg , Pin_Num)	reg=(reg)|(1<<Pin_Num)
#define Get_Pin(reg , Pin_Num)	(reg&(1<<Pin_Num))>>Pin_Num
#define ok 1
#define  error 0
#define SEI() __asm__("SEI")
#define CLI() __asm__("CLI")


#endif /* COMMON _H_ */