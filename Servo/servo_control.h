/**
 * @file		servo_control.h
 * @author		Stian Sandve
 */

#ifndef _SERVO_CONTROL_H_
#define _SERVO_CONTROL_H_

#define MAX_DEFLECTION	2700
#define MIN_DEFLECTION	700
#define CENTER 			1700
#define MOVE			50

typedef enum {
	DIR_CLOCKWISE,
	DIR_COUNTER_CLOCKWISE,
	DIR_CENTER} Direction;

void servo_control_init();
void move_servo(Direction dir);
void set_servo_pos(int pos);
void center_servo();
int get_servo_pos();
void toggle_servo();

#endif
