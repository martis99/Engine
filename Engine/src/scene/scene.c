#include "pch.h"
#include "scene.h"

#include "renderer/renderer.h"

struct Scene {
	byte id;
};

Scene* scene_create(float width, float height) {
	Scene* scene = m_malloc(sizeof(Scene));
	return scene;
}

void scene_delete(Scene* scene) {
	m_free(scene, sizeof(Scene));
}

void scene_update(Scene* scene, float dt) {
	
}

void scene_render(Scene* scene, Renderer* renderer) {

}

void scene_key_pressed(Scene* scene, byte key) {

}

void scene_key_released(Scene* scene, byte key) {

}

void scene_mouse_pressed(Scene* scene, byte button) {

}

void scene_mouse_released(Scene* scene, byte button) {

}

void scene_mouse_moved(Scene* scene, float x, float y) {

}

void scene_mouse_moved_delta(Scene* scene, float dx, float dy) {
	
}

void scene_mouse_wheel(Scene* scene, float delta) {
	
}