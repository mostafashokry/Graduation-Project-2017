/*
 * Timer.h
 *
 * Created: 25/03/2017 08:59:49 م
 *  Author: Shawkat
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#define TCCR0_INIT	0x03
#define TIMSK_INIT	0x01




void Timer_init (void);
void Timer_Reset (void);


#define TOV  97

#endif /* TIMER_H_ */