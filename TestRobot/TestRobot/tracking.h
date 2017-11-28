/*
 * tracking.h
 *
 * Created: 20-Apr-17 6:36:49 PM
 *  Author: ahmed abasery
 */ 


#ifndef TRACKING_H_
#define TRACKING_H_

#define W 0.5
#define cell_L 0.25
#define Kv cell_L
#define l cell_L/2
#define sqrtof2 1.41421356
#define sqrtof2devided 1.010152545


#define end_of_path 1
#define return_to_path_after_OA 2


void move (double vel , double w , int t100ms );
int get_direction(double A[2] , double B[2]);
void update_current_cell ();
void tracking_init();
void track_no_Rotation_calculations ();
int track_no_Rotation();
void track_calc();
int track_slot (int multi_agent);
void modify_after_OA(char OutofPath);

#endif /* TRACKING_H_ */