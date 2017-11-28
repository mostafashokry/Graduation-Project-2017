/*
 * TestRobot.c
 *
 * Created: 2/13/2017 1:10:19 PM
 * Author : Mahmoud
 */

// shokry multiagent

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
#include "obstacle.h"
#include "Ext_Memory.h"
#include "PSD_Man.h"
#include "Common.h"
#include "SPI.h"
#include "kinamtic.h"
#include "tracking.h"
#include "Timer.h"
#define MDPI	6.283185307
#define Shokry	1
#define Afnan	2
//kinematics variables 

//psd & spi variables

extern volatile unsigned int psd[3];	   //Extern from PSD
char Path [100][2]  ;
extern double current_cell[3] ;			//Extern from tracking
extern double current_position [3];		//Extern from tracking
extern char Rotating ;					//Extern from tracking
extern char at_cell_center ;			//Extern from tracking
extern char t_end ;						//Extern from tracking
extern double PoseAtInt[3];				//Extern from PSD
extern volatile char Timer_counter ;	//Extern from timer
volatile char Position [7];				//cell x , cell y , theta , index x , index y , vx , vy 
volatile char path_flag = 0;
int PointFromOA[3];					//new point from obstacle avoidance
char OutOfPath = 0 ;						//obstacle avoidance done
char j =0 ;
volatile char Pose_index=0;
char Num_of_agent = 255;
char Leader = 2;
char Follower_Pose_index =0;
char Follower_pose[]={10,10};
char Follower_flag = 0;
char flip_flop = 0 ; //for making timer 200 msec
char tnr_start = 0 ;
char stop_OA_index = 0 ;
char main_flag = 0 ;
void SPI_Recieve_Path(void)
{
	static char Recieve_flag = 0;
	static char i=0;
	//static char mnm = 0;
	if (SPDR == 254)
	{
		SPDR = Position[Pose_index];
		Pose_index++;
		if (Pose_index==7)
		{
			Pose_index=0;
		}
		return;
	}
	if (Num_of_agent == 255)
	{
		Num_of_agent = SPDR;
		if(main_flag==1)
		{
			Num_of_agent=1;
			main_flag=0;
		}
		return;
	}
	if(Leader == 2)
	{
		Leader=SPDR;
		Follower_Pose_index=0;
		return;
	}
	
	if ((Num_of_agent==1) || (Num_of_agent > 1 && Leader==1))
	{
		if (path_flag==0)
		{
			if (!Recieve_flag)
			{
				Path [i][0]=SPDR;
				Recieve_flag=1;
			}
			else if (Recieve_flag)
			{
				Path[i][1]=SPDR;
				Recieve_flag=0;
				i++;
			}
			if (Path[i-1][0]==255 && Path[i-1][1]==255)
			{
				if (Num_of_agent>1&&Leader==1)
				{
					Position[5]=1;
					Position[6]=1;
				}
				tnr_start = 0;
				path_flag = 1;
				j=0;
				stop_OA_index=i-2;
				i=0;
				Pose_index=0;
			}
		}
	}
	else if(Num_of_agent>1&& Leader==0)
	{
		Follower_pose[Follower_Pose_index] =SPDR;
		Follower_Pose_index++;
		if(Follower_Pose_index==2)
		{
			path_flag =1;
			Follower_Pose_index=0;
			Pose_index=0;
		}
/*
		if(Follower_pose[0]== 0 && Follower_pose[1] == 0)
		{
			path_flag=0;
		}
*/
	}
	return;

}

void Update_position()
{
	update_current_cell();
	Position[0] = current_cell[0];
	Position[1] = current_cell[1];
	Position[2] = round(current_position[2]/6.283185307*255);
	Position[3] = ((int)(current_position[0]*100))%25;
	Position[4] = ((int)(current_position[1]*100))%25;
}

void clear_path (void)
{
	char i=0 ;
	for (i=0 ; i<100 ; i++)
	{
		Path [i][0] = 0;
		Path [i][1] = 0;
	}
}
void main_init()
{
	
	//***initializations***//
	Ext_memo_init();
	AvrInit();
	LcdInit();
	MotorInit();
	init_SPI(SLAVE);
	Set_SPI_Recieve(SPI_Recieve_Path);
	clear_path();
	LedOn(RLED);
	
	psd_init();
	SEI();
	psd_clear();
	
	
	int n = 1;//change for number of robot
	current_cell[0]=1;
	current_position[0]=0.25;
	if (n==1)
	{
		current_cell[1]=1;
		current_position[1]=0.25;
	}
	else if (n==2)
	{
		current_cell[1]=7;
		current_position[1]=1.75;
	}
	else //(n==3)
	{
		current_cell[1]=14;
		current_position[1]=3.50;
	}
	Timer_init();
}
int main(void)
{
	int ma_mode = Shokry ;
	double main_vx=0, main_vy =0,theta;
	int track_result ;
	main_init();
	
	//for test only
	//_delay_ms(4000);
	char iP [2][5] = {{1,1,5,5,-1},{1,5,5,1,-1}};
	for (int e=0 ; e<5 ; e++){Path[e][0]=iP[0][e];Path[e][1]=iP[1][e];} 
	path_flag = 1 ;
	stop_OA_index=100; 
	current_cell[0]=1;current_cell[1]=1;
	Num_of_agent=1;
	Leader=2;
// 	Follower_pose[0]=25;
// 	Follower_pose[1]=25;
	//for test only 
	OutOfPath=0 ; 
	while(1)
	{
		while (!path_flag);
		// inside timer
		if (Timer_counter == TOV)
		{
			if (Num_of_agent==1) //track path
			{
				LedOn(RLED);
				Position[5]=0;
				Position[6]=0;
				// Apply the code here
				if((at_cell_center==1)&&(j<stop_OA_index))//check if the robot at the center of cell
				{
					
					psd_clear();
					psd_read ();
					
					//while( psd[0]==0 || psd[1]==0 || psd[2]==0 );
					//LedOn(GLED);
					_delay_ms(10);
					if (psd[0]!=0&&psd[1]!=0&&psd[2]!=0)LedOn(GLED);
					if ( psd[0]<PSDThreshold || psd[1]<PSDThreshold || psd[2]<PSDThreshold || OutOfPath == 1 )
					{
						MotorStop(MOTOR_STOP_SMOOTH);
						//LedOn(YLED);
						OA ( PointFromOA );
						modify_after_OA(OutOfPath);
					}
					
				}
				if (at_cell_center==1) {at_cell_center=0;} //clear flag
				if (tnr_start==0)
				{
					track_calc();
					tnr_start=1;
				}
				else 
				{
					track_result=track_slot(0);//0 for not updating velocity feedback
					if (track_result==end_of_path)
					{
						tnr_start=0;
						OutOfPath=0;
						tracking_init();
						clear_path();
						LedOn(GLED);
						path_flag=0;
						while(Pose_index!=0);
						Leader=2 ;
						Num_of_agent = 255 ;
						
					}
					else if (track_result==return_to_path_after_OA)
					{
						OutOfPath=0;
						LedOn(GLED);
						tnr_start=0;
					}
				}
			}
			else if (Num_of_agent>1) //multiagent Shokry
			{
				if (ma_mode==Shokry)
				{
					if (Leader==1) //leader
					{
						if (tnr_start==0)
						{
							LedOn(YLED);
							tnr_start=1;
							track_no_Rotation_calculations();
						}
						LedOn(RLED);
						if (track_no_Rotation()==1)
						{
							OutOfPath=0;
							MotorStop(MOTOR_STOP_FAST);
							ApplyVW(0,0);
							tracking_init();
							clear_path();
							LedOn(GLED);
							path_flag=0;
							Leader=2 ;
							Num_of_agent = 255 ;
						}
						
						
					}
					else //follower
					{
						main_flag=1;
						if (flip_flop==1)
						{
							flip_flop=0;
							if (Follower_pose[0]==0&&Follower_pose[1]==0) //leader stopped
							{
								OutOfPath=0;
								MotorStop(MOTOR_STOP_FAST);
								ApplyVW(0,0);
								tracking_init();
								clear_path();
								LedOn(GLED);
								path_flag=0;
								Leader=2 ;
								Num_of_agent = 255 ;
							}
							else
							{
								LedOn(RLED);
								main_vx = (double)Follower_pose[0]/100;
								main_vy = (double)Follower_pose[1]/100;
								ApplyVxVy_double(main_vx,main_vy);
								LedOn(RLED);
								Position[5] = 0;
								Position[6] = 0;
							}
						}
						else{flip_flop=1;}
						current_position[0]+=main_vx*0.1;
						current_position[1]+=main_vy*0.1;
					}
				}
				else if (ma_mode==Afnan)
				{
					if (Leader==1)
					{
						if (at_cell_center==1) {at_cell_center=0;} //clear flag
						if (tnr_start==0)
						{
							track_calc();
							tnr_start=1;
						}
						else
						{
							track_result=track_slot(1); //1 is for update velocity feedback
							if (track_result==end_of_path)
							{
								tnr_start=0;
								OutOfPath=0;
								tracking_init();
								clear_path();
								LedOn(GLED);
								path_flag=0;
								while(Pose_index!=0);
								Leader=2 ;
								Num_of_agent = 255 ;
								
							}
						}
					}
					else //follower
					{
						main_flag=1;
						if (flip_flop==1)
						{
							flip_flop=0;
							if (Follower_pose[0]==0&&Follower_pose[1]==0) //leader stopped
							{
								OutOfPath=0;
								MotorStop(MOTOR_STOP_FAST);
								ApplyVW(0,0);
								tracking_init();
								clear_path();
								LedOn(GLED);
								path_flag=0;
								Leader=2 ;
								Num_of_agent = 255 ;
							}
							else
							{
								LedOn(RLED);
								main_vx = (double)Follower_pose[0]/100;
								main_vy = (double)Follower_pose[1]/100;
								ApplyVW(main_vx,main_vy);
								LedOn(RLED);
								Position[5] = 0;
								Position[6] = 0;
							}
						}
						else{flip_flop=1;}
						theta = current_position[2]+main_vy/2*0.2;//divided by 2 to get average t_theat thought sample
						current_position[0] += main_vx*cos(theta)*0.2;
						current_position[1] += main_vx*sin(theta)*0.2;
						current_position[2] += main_vy*0.2;
					}
				}
			}
			Update_position();
			Timer_Reset();
		}
	}
	while (1) ; 
}
