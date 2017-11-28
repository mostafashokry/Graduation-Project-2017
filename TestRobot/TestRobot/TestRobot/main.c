/*
 * TestRobot.c
 *
 * Created: 2/13/2017 1:10:19 PM
 * Author : Mahmoud
 */



#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "Motor.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

//kinematics macros
#define R 0.0915
#define r 0.03
#define DeltaT 20 //sampling time
#define v 0.1 //10 cm
#define l 0.15
// end of kinematics macros

//kinematics variables 
double profile [8][3] ={  //{Vx  , Yx  ,PhiDot }
							{0.0 , 0.15, 0.0   },
							{0.0 , 0.0 , M_PI/4},
							{0.15, 0.0 , 0.0   },
							{0.0 , 0.0 ,-M_PI/2},
							{0.15, 0.0 , 0.0   },
							{0.0 , 0.0 ,-M_PI/4},
							{0.0 ,-0.15, 0.0   },
							{0.0 , 0.0 , M_PI/2}}; 
double P[5][2]={	  //{x  , y }
					{0.6,0.6},
					{1.2,0.6},
					{1.2,0.9},
					{1.5,1.5},
					{1.5,1.2}};
	
double iprofile [3] ;
double motor_speed [3] ;
double vel_robot[3] ;
double current_position[3] ;
//kinematics variables 

//variables for tracking 



void ApplyRobotVelocities(double velocity [3]);
void move (double vel , double w , int t100ms );

double mV[3];
void move (double vel , double w , int t100ms )
{
	mV[0]=0;mV[1]=vel;mV[2]=w;
	ApplyRobotVelocities(mV);
	for (int mi = 0 ; mi<t100ms ; ++mi) _delay_ms(100);
}

double ttheta ,ttheta_nxt ,ttheta_dfr,tr,tW;
void track_points()
{
	ttheta=atan((P[0][1]-current_position[1])/(P[0][0]-current_position[0]));
	if (P[0][0]-current_position[0]<0){
		if (ttheta>0) ttheta-=M_PI;
		else ttheta+=M_PI;}
	tr=sqrt((P[0][0]-current_position[0])*(P[0][0]-current_position[0])+(P[0][1]-current_position[1])*(P[0][1]-current_position[1]));
	move(0,ttheta/2,20);
	move(v,0,round(10*(tr-l)/v));
	for (int i=0 ; i<3 ; ++i)
	{
		ttheta_nxt=atan((P[i+1][1]-P[i][1])/(P[i+1][0]-P[i][0]));
		if (P[i+1][0]<P[i][0]){
			if (ttheta_nxt>0) ttheta_nxt-=M_PI;
			else ttheta_nxt+=M_PI;}
		ttheta_dfr=ttheta_nxt-ttheta;
		tr=floor(sqrt(((P[i+1][0]-P[i][0])*(P[i+1][0]-P[i][0]))+((P[i+1][1]-P[i][1])*(P[i+1][1]-P[i][1]))));
		tW=v/(l*(1-cos(ttheta_dfr))/sin(ttheta_dfr));
		move(v,tW,round(10*ttheta_dfr/tW));
		move(v,0,round(10*(tr-2*l)/v));
		ttheta=ttheta_nxt;
	}
	
}

void ApplyVelocities (double AV [3])
{
	
	//vel_robot is velocities relative to robot
	vel_robot[0] = cos(current_position[2])*AV[0]+sin(current_position[2])*AV[1];
	vel_robot[1] = -1*sin(current_position[2])*AV[0]+cos(current_position[2])*AV[1];
	vel_robot[2] = AV[2];
	
	ApplyRobotVelocities(vel_robot);
}

double ARV[3];
void ApplyRobotVelocities(double velocity [3])
{
	ARV[2]=velocity[2];
	ARV[1]=-velocity[1]*0.8660254038-velocity[0]*0.5;
	ARV[0]=velocity[1]*0.5-velocity[0]*0.8660254038;


	//correction factor
	iprofile[0]=ARV[0]*M_PI;
	iprofile[1]=ARV[1]*M_PI;
	iprofile[2]=ARV[2]*3.05*0.969;
	
	motor_speed [0]=(0.866025*iprofile[0]-0.5*iprofile[1]+R*iprofile[2])/r;
	motor_speed [1]=(iprofile[1]+R*iprofile[2])/r;
	motor_speed [2]=(-0.866025*iprofile[0]-0.5*iprofile[1]+R*iprofile[2])/r;

	MotorStart(motor_speed);
}
int main(void)
{
// 	char x [5] ;
	
	//***initializations***//
	for (int i=0 ; i<3 ; ++i) current_position[i]=0;
	AvrInit();
	MotorInit();
	LcdInit();
	//***initializations***//
	
	LcdClear();
	LcdString(LCD_LINE1, "Motor Test Program");
	LedOn(GLED);
		
	while(1)
	{
		track_points();
		MotorStop(MOTOR_STOP_FAST);
// 		LcdClear();
// 		itoa(current_position[2]*180/M_PI,x,10);
// 		LcdString(LCD_LINE1,"DONE");
		_delay_ms(3000);
	}
}
