#include "keyboard.h"

bool key_states[256];

void kb_key_pressed(byte key)
{
	key_states[key] = 1;
}

void kb_key_released(byte key)
{
	key_states[key] = 0;
}

bool is_key_pressed(byte key)
{
	return key_states[key];
}
