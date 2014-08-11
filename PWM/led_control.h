/**
 * @file		servo_control.h
 * @author		Stian Sandve
 */

#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

typedef enum {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE} Color;

void led_control_init();
void toggle_led();

#endif
