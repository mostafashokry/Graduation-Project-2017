/*
 * obstacle.c
 *
 * Created: 4/10/2017 10:58:33 AM
 *  Author: Afnaan
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define N 4 //no. of points in path, checked to next
#define m 3
#define pi 3.1415926
#define pi2 1.570796

extern char Path[100][2] ;	// extern from main file path from spi
extern char OutOfPath ;		// extern from main file Obstacle Avoidance flag
extern volatile unsigned int psd[3];	   //Extern from PSD        names from left 1 0 2(module)
extern double current_cell[3] ;			//Extern from tracking
double Theta_ed ;
extern char j ;				//  extern from main file index of path

void ConvToUnit ( double NotOnes[2], signed short Oness[2]);

/* directions
    -1,1    0,1     1,1
    -1,0    0,0     1,0
    -1,-1   0,-1    1,-1
*/

// index of path +1 to find next
int WhichNext ( char j  , int NextPoint[3])
{
    int i,index=0;
    double Dist ,MiniDist=1000;
    if ( abs(Path[j+1][0]-current_cell[0])<2 && abs(Path[j+1][1]-current_cell[1])<2 && psd[0]!=80 && psd[1]!=80 && psd[2]!=80 )
            { j++; }
    for(i=(j+1);i<(m+j);i++)		// i=j+1 be cause path[j] is already passed
    {
        if (Path[i][0]==255||Path[i][1]==255) {break;} // obstacle is goal :D    && or ||
		Dist = sqrt( pow( (current_cell[0]-Path[i][0]),2) + pow( (current_cell[1]-Path[i][1]),2) );
        if (Dist<MiniDist)
        {
            MiniDist = Dist ;
            index=i;
        }
    }
    NextPoint[0] = Path[index][0];
    NextPoint[1] = Path[index][1];
	return (index-1);
}

// determine direction of next point
void GetDir (int NextPoint[3],signed short dir[2])
{
    double NPr[2];  // next point in respect to robot
    signed int Dist[3];    // distance between current pose and next point on path
    Dist[0] = NextPoint[0] - current_cell[0];
    Dist[1] = NextPoint[1] - current_cell[1];
    {//added by ahmed
        if (Dist[0]>0.05) Dist[0]=1;
        else if (Dist[0]<-0.05) Dist[0]=-1;
        else Dist[0]=0;
        if (Dist[1]>0.05) Dist[1]=1;
        else if (Dist[1]<-0.05) Dist[1]=-1;
        else Dist[1]=0;
    }
    NPr[0] =  Dist[0]*cos(current_cell[2]) + Dist[1]*sin(current_cell[2]);
    NPr[1] = -1*Dist[0]*sin(current_cell[2]) + Dist[1]*cos(current_cell[2]);
    if (NPr[0]>0.05) //edited from 0 to 0.4
        dir[0] = 1;
    else if (NPr[0]<-0.05)
        dir[0] = -1;
    else
        dir[0] = 0;
    if (NPr[1]>0.05)
        dir[1] = 1;
    else if (NPr[1]<-0.05)
        dir[1] = -1;
    else
        dir[1] = 0;
}

//  determine direction should be in new point
int GetTheta (short dir[2])
{
	int Theta=0 ;
	if (dir[0] == 1 && dir[1] == 0)
	{
		Theta = 0 ;
	}
	else if (dir[0] == 1 && dir[1] == 1)
	{
		Theta = 1 ;	// pi/4 ;
	}
	else if (dir[0] == 0 && dir[1] == 1)
	{
		Theta = 2 ;	// pi/2 ;
	}
	else if (dir[0] == -1 && dir[1] == 1)
	{
		Theta = 3 ;	// 3*pi/4 ;
	}
	else if (dir[0] == -1 && dir[1] == 0)
	{
		Theta = 4 ;	// pi ;
	}
	else if (dir[0] == -1 && dir[1] == -1)
	{
		Theta = 5 ;	// 5*pi/4 ;
	}
	else if (dir[0] == 0 && dir[1] == -1)
	{
		Theta = 6 ;	// 3*pi/2 ;
	}
	else if (dir[0] == 1 && dir[1] == -1)
	{
		Theta = 7 ; //	7*pi/4 ;
	}
	return Theta ;
}

// fn converts from robot frame to world frame  (converts directions)
void ConvToUnit ( double NotOnes[2], signed short Oness[2])
{
	if (NotOnes[0]>0.05)
        Oness[0] = 1;
    else if (NotOnes[0]<-0.05)
        Oness[0] = -1;
    else
        Oness[0] = 0;
    if (NotOnes[1]>0.05)
        Oness[1] = 1;
    else if (NotOnes[1]<-0.05)
        Oness[1] = -1;
    else
        Oness[1] = 0;
}

void OA ( int NewPoint[3])
{
    //Theta_ed = current_cell[2] - pi2 ;
	signed short NP_dir[2] /*,NP_DToRU[2]*/, DirToWU[2], NextP2NewP_dirU[2];
	// NP_dir : direction of next point in respect to current position
	// NP_DToRU : next point direction in robot frame (unit value)
	// DirToWU : total direction will be applied in world frame (unit value)
	// NextP2NewP_dirU : direction of next point in respect to new point (unit value)
    double num[2] /*, NP_dirToR[2] */, DirToR[2], DirToW[2], NextP2NewP_dir[2];
	signed int denum[2] ;
	// DirToR : direction in respect to robot & DirToW : to world
    // num : numerator of DirToR (sum of all directions )
    int NextPoint[3];

    j = WhichNext( j , NextPoint);
    //printf("NextPoint %d %d \t\n",NextPoint[0],NextPoint[1]);

    GetDir( NextPoint, NP_dir);//this function return NP_dir with respect to (((robot)))
    //printf("NP_dir %d  %d \n",NP_dir[0],NP_dir[1]);

        /*  directions biased by sensors
        sensor 1 on left   directs to ( 1, 0)
        sensor 0 in middle directs to ( 0,-1)
        sensor 2 on right  directs to (-1, 0)     */

    do
    {
        num[0] = (0.5*NP_dir[0] + (1-(psd[1]/80.0))-(1-(psd[2]/80.0)));
        num[1] = (0.5*NP_dir[1] - (1-(psd[0]/80.0) ) );
        denum[0] = ( 1+ ceil(1-(psd[1]/80.0))+ceil(1-(psd[2]/80.0)) );
        denum[1] = ( 1+ceil(1-(psd[0]/80.0)) );

        //printf("DirToRx %f / %d \n",num[0],denum[0]);
        //printf("DirToRy %f / %d \n",num[1],denum[1]);

        DirToR[0] = num[0] / denum[0] ;
        DirToR[1] = num[1] / denum[1] ;
        //printf("DirToR %f  %f \n",DirToR[0],DirToR[1]);
        {
            if (DirToR[0]>0.05) DirToR[0]=1;
            else if (DirToR[0]<-0.05) DirToR[0]=-1;
            else DirToR[0]=0;
            if (DirToR[1]>0.05) DirToR[1]=1;
            else if (DirToR[1]<-0.05) DirToR[1]=-1;
            else DirToR[1]=0;
        }
        if( DirToR[0]==0 && DirToR[1]<0 ) 		// to prevent back directions
        {
            j++;
            j = WhichNext( j , NextPoint);
            //printf("NextPoint %d %d \t\n",NextPoint[0],NextPoint[1]);

            GetDir( NextPoint, NP_dir);//this function return NP_dir with respect to (((robot)))
            //printf("NP_dir %d  %d \n",NP_dir[0],NP_dir[1]);
            j--;
        }
        else if(DirToR[1]<0)
        { DirToR[1]=0; }
    }
    while( DirToR[0]==0 && DirToR[1]<0 );

    DirToW[0] = DirToR[0]*cos(current_cell[2])-DirToR[1]*sin(current_cell[2]);
    DirToW[1] = DirToR[0]*sin(current_cell[2])+DirToR[1]*cos(current_cell[2]);
     //printf("DirToW %f  %f \n",DirToW[0],DirToW[1]);

    ConvToUnit ( DirToW, DirToWU );
     //printf("DirToWU %d  %d \n",DirToWU[0],DirToWU[1]);
	NewPoint[0] = current_cell[0] + DirToWU[0] ;
    NewPoint[1] = current_cell[1] + DirToWU[1] ;

    NextP2NewP_dir[0] = Path[j+1][0]- NewPoint[0] ;
    NextP2NewP_dir[1] = Path[j+1][1]- NewPoint[1] ;
    ConvToUnit ( NextP2NewP_dir, NextP2NewP_dirU );

	NewPoint[2] = GetTheta(NextP2NewP_dirU);

	// to continue in OA till returning to path
	if((Path[j+1][0]-Path[j][0])==0) // x1 = x2 , mile inf
	{
		// x == (x1 || x2)
		if( NewPoint[0] == Path[j][0] )
		OutOfPath=0;
		else
		OutOfPath=1;
	}
	else
	{
		// y == (y2-y1)*(x-x1)/(x2-x1) + y1
		if( NewPoint[1] == (Path[j+1][1]-Path[j][1])*(NewPoint[0]-Path[j][0])/(Path[j+1][0]-Path[j][0]) + Path[j][1] )
		OutOfPath=0;
		else
		OutOfPath=1;
	}
}
