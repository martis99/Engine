#pragma once
#include "eng_common_types.h"

void ms_button_pressed(byte button);
void ms_button_released(byte button);
void ms_moved(float x, float y);
void ms_moved_delta(float x, float y);
void ms_mouse_wheel(float delta);

bool is_mouse_left_pressed();
bool is_mouse_right_pressed();
bool is_mouse_middle_pressed();

float get_mouse_x();
float get_mouse_y();