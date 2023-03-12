#include "mouse.h"
#include "keys.h"

bool left_is_pressed   = 0;
bool right_is_pressed  = 0;
bool middle_is_pressed = 0;
float mouse_x	       = 0;
float mouse_y	       = 0;
float mouse_dx	       = 0;
float mouse_dy	       = 0;
float wheel_delta      = 0;

void ms_button_pressed(byte button)
{
	switch (button) {
	case K_MOUSEL: left_is_pressed = 1; break;
	case K_MOUSER: right_is_pressed = 1; break;
	case K_MOUSEM: middle_is_pressed = 1; break;
	}
}

void ms_button_released(byte button)
{
	switch (button) {
	case K_MOUSEL: left_is_pressed = 0; break;
	case K_MOUSER: right_is_pressed = 0; break;
	case K_MOUSEM: middle_is_pressed = 0; break;
	}
}

void ms_moved(float x, float y)
{
	mouse_x = x;
	mouse_y = y;
}

void ms_moved_delta(float dx, float dy)
{
	mouse_dx = dx;
	mouse_dy = dy;
}

void ms_mouse_wheel(float delta)
{
	wheel_delta = delta;
}

bool is_mouse_left_pressed()
{
	return left_is_pressed;
}

bool is_mouse_right_pressed()
{
	return right_is_pressed;
}

bool is_mouse_middle_pressed()
{
	return middle_is_pressed;
}

float get_mouse_x()
{
	return mouse_x;
}

float get_mouse_y()
{
	return mouse_y;
}
