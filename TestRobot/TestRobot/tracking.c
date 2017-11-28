/*
 * tracking.c
 *
 * Created: 20-Apr-17 6:40:00 PM
 *  Author: ahmed abasery
 */ 


#include "tracking.h"
#include "kinamtic.h"
#include "Motor.h"
#include "led.h"
#include <math.h>

extern char Path [100][2];

double P[5][2]={	  //{x  , y }
{0.6,0.6},
{1.2,0.6},
{1.2,1.2},
{1.5,1.2},
{1.8,1.5}};
	

double current_position[3] ;
double current_cell [3];
int t_cntr_forward , t_cntr ;
double t_v ,t_w  ,t_r , t_theta ;
int t_direction =0, t_direction_nxt , t_direction_difr;
double t_P [2][2];
char t_Start = 0 , Rotating=0 , at_cell_center = 0 , t_end =0 ,t_OA =0 , t_OA_end =0 , t_change_velocities = 0 ;
extern char j;	
extern int PointFromOA[3] ;
extern volatile char Position [7];


//psd & spi variables





int get_direction(double A[2] , double B[2])
{
	/* Directions
	3	2	1
	4	*	0
	5	6	7
	*/
	if (B[0]>A[0])
	{
		if (B[1]>A[1]) return 1 ; //up right
		else if (B[1]<A[1]) return 7 ; //down right
		else return 0 ; //right
	}
	if (B[0]<A[0])
	{
		if (B[1]>A[1]) return 3 ; //up left
		else if (B[1]<A[1]) return 5 ; //down left
		else return 4 ; //left
	}
	else
	{
		if (B[1]>A[1]) return 2 ; //up
		else return 6 ; //down
	}
}




void update_current_cell ()
{
	current_cell[0]=round(current_position[0]/cell_L);
	current_cell[1]=round(current_position[1]/cell_L);
	current_cell[2]=t_direction*M_PI/4-1.5708;
} 

void tracking_init()
{
	j=0 ; 
	t_Start = 0 ;
	t_change_velocities=0;
}





int tnr_cntr , tnr_dir , tnr_diffrence , tnr_profile_pointer=0;
double tnr_vx , tnr_vy ;
int tnr_difr[2] ;
double tnr_velocity_profile [100][2];
int tnr_vpcntr;
int tnr_velocity_cntr_profile [100];
int tnr_i;

void add_to_profile (double vvx , double vvy , int vcntr)
{
	tnr_velocity_profile[tnr_vpcntr][0]=vvx;
	tnr_velocity_profile[tnr_vpcntr][1]=vvy;
	tnr_velocity_cntr_profile[tnr_vpcntr]=vcntr;
	tnr_vpcntr++;
}
void add_to_profile_direction(double vv , int vcntr)
{
	switch(tnr_dir)
	{
		case 0 :
		{
			add_to_profile(vv,0,vcntr);
			break;
		}
		case 1 :
		{
			add_to_profile(vv,vv,vcntr);
			break;
		}
		case 2 :
		{
			add_to_profile(0,vv,vcntr);
			break;
		}
		case 3:
		{
			add_to_profile(-vv,vv,vcntr);
			break;
		}
		case 4 :
		{
			add_to_profile(-vv,0,vcntr);
			break;
		}
		case 5 :
		{
			add_to_profile(-vv,-vv,vcntr);
			break;
		}
		case 6 :
		{
			add_to_profile(0,-vv,vcntr);
			break;
		}
		case 7 :
		{
			add_to_profile(vv,-vv,vcntr);
			break;
		}
	}
}

int test ;
void track_no_Rotation_calculations ()
{
	tnr_i=0;
	tnr_vpcntr=0;
	tnr_profile_pointer=0;
	test=0;
	while(1)
	{
		t_P[0][0] =Path[tnr_i][0];
		t_P[0][1] =Path[tnr_i][1];
		t_P[1][0] =Path[tnr_i+1][0];
		t_P[1][1] =Path[tnr_i+1][1];
		tnr_dir= get_direction(t_P[0],t_P[1]);
		tnr_difr[0] = Path[tnr_i+1][0]-Path[tnr_i][0];
		tnr_difr[1] = Path[tnr_i+1][1]-Path[tnr_i][1];
		if (tnr_difr[0]>0)
		{tnr_diffrence = tnr_difr[0];}
		else if (tnr_difr[0]<0)
		{tnr_diffrence = -tnr_difr[0];}
		else
		{
			if (tnr_difr[1]>0)
			{tnr_diffrence = tnr_difr[1];}
			else
			{tnr_diffrence = -tnr_difr[1];}
		}
		if (tnr_diffrence==1) // distance is exactly one cell
		{
			add_to_profile_direction(0.10,05);
	
			add_to_profile_direction(0.15,10);
			
			add_to_profile_direction(0.10,05);
		}
		else //distance is bigger than 1 cell
		{
			add_to_profile_direction(0.05,05);
			
			add_to_profile_direction(0.10,05);
			
			add_to_profile_direction(0.15,05);
			
			add_to_profile_direction(0.20,05);
			
			if (tnr_diffrence>2)
			{
				add_to_profile_direction(0.25,10*(tnr_diffrence-2));
				
			}
			add_to_profile_direction(0.20,05);
			
			add_to_profile_direction(0.15,05);
			
			add_to_profile_direction(0.10,05);
			
			add_to_profile_direction(0.05,05);
			test+=5;
		}
		tnr_i++;
		if (Path[tnr_i+1][0]==-1||Path[tnr_i+1][0]==255){return;}
	}
}

int track_no_Rotation()
{
	
	if (tnr_cntr==0)
	{
		if (tnr_profile_pointer==tnr_vpcntr) 
		{
// 			MotorStop(MOTOR_STOP_FAST);
// 			ApplyVxVy_double(0,0);
			Position[5]=0;
			Position[6]=0;
			return 1;
		}
		tnr_cntr=tnr_velocity_cntr_profile[tnr_profile_pointer];
		ApplyVxVy_double(tnr_velocity_profile[tnr_profile_pointer][0],tnr_velocity_profile[tnr_profile_pointer][1]);
		Position[5]=tnr_velocity_profile[tnr_profile_pointer][0]*100;
		Position[6]=tnr_velocity_profile[tnr_profile_pointer][1]*100;
		tnr_profile_pointer++;
	}
	tnr_cntr--;
	current_position[0]+=0.1*tnr_velocity_profile[tnr_profile_pointer-1][0];
	current_position[1]+=0.1*tnr_velocity_profile[tnr_profile_pointer-1][1];
	return 0;
}
double tr_profile [100][2] ;
int tr_profile_details [100][2] ; //[time slots required , applied j]
int tr_profile_cntr , tr_profile_it; 
void tr_add_to_profile (double V , double w , int slots )
{
	tr_profile[tr_profile_cntr][0]=V;
	tr_profile[tr_profile_cntr][1]=w;
	tr_profile_details[tr_profile_cntr][0]=slots;
	tr_profile_details[tr_profile_cntr][1]=j;
	tr_profile_cntr++;
}
void tr_add_to_profile_end_after_OA(char Y)
{
	if (Y==0)
		{tr_add_to_profile(1.0,1.0,1);}
	else if (Y==1)
	{
		tr_add_to_profile(2.0,2.0,2);
	}
}
int check_end_after_OA ()
{
	if (tr_profile[tr_profile_it][0]==1.0&&tr_profile[tr_profile_it][1]==1.0&&tr_profile_details[tr_profile_it][0]==1) return 1 ;
	else if (tr_profile[tr_profile_it][0]==2.0&&tr_profile[tr_profile_it][1]==2.0&&tr_profile_details[tr_profile_it][0]==2) return 2 ;
	return 0 ;
}
void modify_after_OA(char OutofPath)
{
	t_OA=1;
	tr_profile_cntr=0;
	tr_profile_it=-1; //incremented first befour being used
	t_cntr=0;
	t_P[0][0] =current_cell[0]*cell_L;
	t_P[0][1] =current_cell[1]*cell_L;
	t_P[1][0] =PointFromOA[0]*cell_L;
	t_P[1][1] =PointFromOA[1]*cell_L;
	t_direction_nxt = get_direction(t_P[0],t_P[1]);
	t_direction_difr = t_direction_nxt - t_direction ;
	if (t_direction_difr<-3) t_direction_difr += 8 ;
	if (t_direction_difr>4) t_direction_difr -= 8 ;
	if(t_direction_difr>0) tr_add_to_profile(0, M_PI/4 , t_direction_difr*10);
	else  if (t_direction_difr<0)tr_add_to_profile(0, -M_PI/4 , -t_direction_difr*10);
	t_r=sqrt(((t_P[1][0]-t_P[0][0])*(t_P[1][0]-t_P[0][0]))+((t_P[1][1]-t_P[0][1])*(t_P[1][1]-t_P[0][1])));
	if(t_direction_nxt%2==0)
	t_cntr_forward=round(10*t_r/Kv);
	else
	t_cntr_forward=round(10*t_r/(sqrtof2devided*Kv));
	t_direction=t_direction_nxt;
	if (t_cntr_forward!=0)
	{
		if (t_direction%2==0) //  left , up , right , down
		tr_add_to_profile(Kv,0,t_cntr_forward);
		else // other directions
		tr_add_to_profile(sqrtof2devided*Kv,0,t_cntr_forward);
	}
	t_direction_nxt = PointFromOA[2] ;
	t_direction_difr = t_direction_nxt - t_direction ;
	if (t_direction_difr<-3) t_direction_difr += 8 ;
	if (t_direction_difr>4) t_direction_difr -= 8 ;
	if(t_direction_difr>0) tr_add_to_profile(0, M_PI/4 , t_direction_difr*10);
	else if (t_direction_difr<0) tr_add_to_profile(0, -M_PI/4 , -t_direction_difr*10);
	tr_add_to_profile_end_after_OA(OutofPath);
}
void track_calc ()
{
	tr_profile_cntr=0;
	tr_profile_it=-1; //incremented first befour being used
	t_cntr=0;
	t_P[0][0] =current_cell[0]*cell_L;
	t_P[0][1] =current_cell[1]*cell_L;
	t_P[1][0] =Path[j+1][0]*cell_L;
	t_P[1][1] =Path[j+1][1]*cell_L;
	
	t_direction_nxt = get_direction(t_P[0],t_P[1]);
	t_direction_difr = t_direction_nxt - t_direction ;
	if (t_direction_difr<-3) t_direction_difr += 8 ;
	if (t_direction_difr>4) t_direction_difr -= 8 ;
	if(t_direction_difr>0) tr_add_to_profile(0, M_PI/4 , t_direction_difr*10);
	else if (t_direction_difr<0) tr_add_to_profile(0, -M_PI/4 , -t_direction_difr*10);

	t_r=sqrt(((t_P[1][0]-t_P[0][0])*(t_P[1][0]-t_P[0][0]))+((t_P[1][1]-t_P[0][1])*(t_P[1][1]-t_P[0][1])));
	if(t_direction_nxt%2==0)
	{
		t_cntr_forward=round(10*(t_r-l)/Kv);
		tr_add_to_profile(Kv,0,t_cntr_forward);
	}
	else
	{
		t_cntr_forward=round(10*(t_r-l)/(sqrtof2devided*Kv));
		tr_add_to_profile(sqrtof2devided*Kv,0,t_cntr_forward);
	}
	t_direction=t_direction_nxt;
	while (((signed char)Path[j+2][0]!=-1)&&(Path[j+2][0]!=255))
	{
		j++;
		t_P[0][0] =Path[j][0]*cell_L;
		t_P[0][1] =Path[j][1]*cell_L;
		t_P[1][0] =Path[j+1][0]*cell_L;
		t_P[1][1] =Path[j+1][1]*cell_L;
		t_direction_nxt = get_direction(t_P[0],t_P[1]);
		t_direction_difr = t_direction_nxt - t_direction +4 ;
		if (t_direction_difr<0) t_direction_difr += 8 ;
		if (t_direction_difr>7) t_direction_difr -= 8 ;
		switch (t_direction_difr)
		{
			/*
			case 2 : set_vwcntr (l*M_PI/8,-M_PI/8,40) ; break ; // go right
			case 3 : set_vwcntr (l*2.41421*M_PI/8,-M_PI/8,20) ; break ; // go up right
			case 5 : set_vwcntr (l*2.41421*M_PI/8,M_PI/8,20) ; break ; // go up left
			case 6 : set_vwcntr (l*M_PI/8,M_PI/8,40) ; break ; // go left
			*/
			
			case 2 : tr_add_to_profile (4*l*M_PI/8,-M_PI/8*4,10) ; break ; // go right
			case 3 : tr_add_to_profile (2*l*2.41421*M_PI/8,-M_PI/8*2,10) ; break ; // go up right
			case 5 : tr_add_to_profile (2*l*2.41421*M_PI/8,M_PI/8*2,10) ; break ; // go up left
			case 6 : tr_add_to_profile (4*l*M_PI/8,M_PI/8*4,10) ; break ; // go left
		}
		t_r=sqrt(((t_P[1][0]-t_P[0][0])*(t_P[1][0]-t_P[0][0]))+((t_P[1][1]-t_P[0][1])*(t_P[1][1]-t_P[0][1])));
		if(t_direction_nxt%2==0)
			t_cntr_forward=round(10*(t_r-2*l)/Kv);
		else
			t_cntr_forward=round(10*(t_r-2*l)/(sqrtof2devided*Kv));
		
		t_direction=t_direction_nxt;
		if (t_cntr_forward!=0)
		{
			if (t_direction%2==0) //  left , up , right , down
				tr_add_to_profile(Kv,0,t_cntr_forward);
			else // other directions
				tr_add_to_profile(sqrtof2devided*Kv,0,t_cntr_forward);
		}
	}
	if (t_direction%2==0)
	{tr_add_to_profile(Kv,0,5);}
	else
	{tr_add_to_profile(Kv*sqrtof2devided,0,7);}
}
int check_result ;
int track_slot (int multi_agent)
{
	if (t_cntr==0)
	{
		tr_profile_it++;
		if (tr_profile_it==tr_profile_cntr)
		{
			MotorStop(MOTOR_STOP_SMOOTH);
			update_current_cell();
			return end_of_path ;
		}
		check_result=check_end_after_OA();
		if (check_result==1)
		{
			MotorStop(MOTOR_STOP_SMOOTH);
			update_current_cell();
			t_OA=0;
			return return_to_path_after_OA ;
		}
		else if (check_result==2)
		{
			update_current_cell();
			at_cell_center=1;
			return 0 ;
		}
		ApplyVW(tr_profile[tr_profile_it][0],tr_profile[tr_profile_it][1]);
		if (multi_agent==1)
		{
			Position[5]=tr_profile[tr_profile_it][0]*100;
			Position[6]=tr_profile[tr_profile_it][1]*100;
		}
		t_cntr=tr_profile_details[tr_profile_it][0];
		j=tr_profile_details[tr_profile_it][1];
		if (tr_profile[tr_profile_it][1]!=0) Rotating=1;
		else Rotating=0;
	}
	t_theta = current_position[2]+tr_profile[tr_profile_it][1]/2*0.1;//divided by 2 to get average t_theat thought sample
	current_position[0] += tr_profile[tr_profile_it][0]*cos(t_theta)*0.1;
	current_position[1] += tr_profile[tr_profile_it][0]*sin(t_theta)*0.1;
	current_position[2] += tr_profile[tr_profile_it][1]*0.1;
	t_cntr--;
	if(Rotating==0&&t_OA==0) // not start OA while OA working
	{
		if(t_direction%2==0)
		{
			if ((t_cntr-5)%10==0)
			{
				at_cell_center=1;
				update_current_cell();
			}
		}
		else
		{
			if ((t_cntr-9)%14==0)
			{
				at_cell_center=1;
				update_current_cell();
			}
		}
	}
	return 0; 
}
