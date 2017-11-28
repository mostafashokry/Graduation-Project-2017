/*
 * kinematic.c
 *
 * Created: 4/11/2017 10:56:14 AM
 *  Author: Afnaan
 */ 
#include "kinamtic.h"
#include "lcd.h"
#include <math.h>
#include "motor.h"
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


double iprofile [3] ;
double motor_speed [3] ;
double vel_robot[3] ;

double ARV[3];
double mV[3];
extern double current_position[3] ;


void ApplyRobotVelocities(double velocity [3])
{
	ARV[2]=velocity[2];
	ARV[1]=-velocity[1]*0.8660254038-velocity[0]*0.5;
	ARV[0]=velocity[1]*0.5-velocity[0]*0.8660254038;


	//correction factor
	iprofile[0]=ARV[0]*M_PI*0.96774193548387096774193548387097;
	iprofile[1]=ARV[1]*M_PI*0.96774193548387096774193548387097;
	iprofile[2]=ARV[2]*3.05*0.969;
	
	motor_speed [0]=(0.8660254038*iprofile[0]-0.5*iprofile[1]+R*iprofile[2])/r;
	motor_speed [1]=(iprofile[1]+R*iprofile[2])/r;
	motor_speed [2]=(-0.8660254038*iprofile[0]-0.5*iprofile[1]+R*iprofile[2])/r;

	MotorStart(motor_speed);
}

void ApplyVelocities (double AV [3])
{
	
	//vel_robot is velocities relative to robot
	vel_robot[0] = cos(current_position[2])*AV[0]+sin(current_position[2])*AV[1];
	vel_robot[1] = -1*sin(current_position[2])*AV[0]+cos(current_position[2])*AV[1];
	vel_robot[2] = AV[2];
	
	ApplyRobotVelocities(vel_robot);
}

void ApplyVW(double velo , double w)
{
	mV[0]=0;mV[1]=velo;mV[2]=w;
	ApplyRobotVelocities(mV);
}

void ApplyVxVy(char VVVx  , char VVVy )
{
	mV[0]=-(double)VVVy/100;mV[1]=(double)VVVx/100;mV[2]=0; // convert from cm/s to m/s //
	ApplyVelocities(mV); 
	
	
	/*
 VVVy^
	 |
	 |
	 |
	Robot==>________>VVVx
	*/
}
void ApplyVxVy_double(double VVVx , double VVVy )
{
	mV[0]=-VVVy;mV[1]=VVVx;mV[2]=0; 
	ApplyVelocities(mV);
}