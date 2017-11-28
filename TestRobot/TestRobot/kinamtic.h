/*
 * kinamtic.h
 *
 * Created: 4/11/2017 10:57:44 AM
 *  Author: Afnaan
 */ 


#ifndef KINAMTIC_H_
#define KINAMTIC_H_

//kinematics macros
#define R 0.0915
#define r 0.03
#define DeltaT 20 //sampling time

// end of kinematics macros


void ApplyRobotVelocities(double velocity [3]);
void ApplyVelocities (double AV [3]);
void ApplyVW(double velo , double w);
void ApplyVxVy(char Vx  , char Vy );
void ApplyVxVy_double(double VVVx , double VVVy );


#endif /* KINAMTIC_H_ */