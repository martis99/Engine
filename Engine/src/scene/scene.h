#pragma once
#include "types.h"

Scene* scene_create(int width, int height);
void scene_delete(Scene* scene);

void scene_main_loop(Scene* scene);

void scene_update(Scene* scene, float dt);
void scene_render(Scene* scene);

void scene_key_pressed(Scene* scene, byte key);
void scene_key_released(Scene* scene, byte key);
void scene_mouse_pressed(Scene* scene, byte button);
void scene_mouse_released(Scene* scene, byte button);
void scene_mouse_moved(Scene* scene, float x, float y);
void scene_mouse_moved_delta(Scene* scene, float dx, float dy);
void scene_mouse_wheel(Scene* scene, float delta);

void scene_exit(Scene* scene);