#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED



#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define LMD18200_OUTPUT    (*(volatile unsigned char *)0x4a00)
#define MOTOR1_CMD        (*(volatile unsigned char *)0x4000)
#define MOTOR1_DATA        (*(volatile unsigned char *)0x4100)
#define MOTOR2_CMD        (*(volatile unsigned char *)0x4200)
#define MOTOR2_DATA        (*(volatile unsigned char *)0x4300)
#define MOTOR3_CMD        (*(volatile unsigned char *)0x4400)
#define MOTOR3_DATA        (*(volatile unsigned char *)0x4500)

// LM629 command
#define STT        0x01
#define UDF        0x04
#define RSTI    0x1d
#define LFIL    0x1e
#define LTRJ    0x1f

// 3 motor numbers of robot
#define MOTOR1    0
#define MOTOR2    1
#define MOTOR3    2

#define MOTOR_F    0
#define MOTOR_B    1

#define MOTOR_STOP_FAST     0
#define MOTOR_STOP_OFF        1
#define MOTOR_STOP_SMOOTH    2

#define ROBOT_RADIUS (25.5/2)

#define LINE_CENTER        0
#define LINE_LEFT0        1
#define LINE_LEFT1        2
#define LINE_LEFT2        3
#define LINE_LEFT3        4
#define LINE_RIGHT0        5
#define LINE_RIGHT1        6
#define LINE_RIGHT2        7
#define LINE_RIGHT3        8


// Declare the prototype of function using Motor
void WriteData(char motor, unsigned char data);
void WriteCmd(char motor, unsigned char cmd);
unsigned char ReadStatus(char motor);
void BusybitCheck(char motor);
void WriteData2(char motor, unsigned int data);
void MotorInit();
void PIDSetting(char motor, unsigned int P, unsigned int I, unsigned int D);
void MotorReset(char motor);
void MotorDir(char motor, char dir);
void MotorStop(unsigned char stop);
void STTMotor();
void AccSet(char motor, double acc);
void VeloSet(char motor, double speed);
void MotorStart(double *v);
void AvrInit();



#endif // MOTOR_H_INCLUDED
