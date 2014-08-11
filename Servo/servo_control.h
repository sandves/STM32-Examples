/**
 * @file		servo_control.h
 * @author		Stian Sandve
 */

#ifndef _SERVO_CONTROL_H_
#define _SERVO_CONTROL_H_

#define DIR_CLOCKWISE			0
#define DIR_COUNTER_CLOCKWISE	1
#define DIR_CENTER				2
#define MAX_DEFLECTION	2750
#define MIN_DEFLECTION	750
#define CENTER 			1750
#define MOVE			50

void servo_control_init();
void move_servo(int dir);
void set_servo_pos(int pos);
void center_servo();
int get_servo_pos();
void toggle_servo();

#endif
