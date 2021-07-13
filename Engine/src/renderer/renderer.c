#include "pch.h"
#include "renderer.h"

#include "app.h"

#include "assets/shader.h"
#include "assets/mesh.h"

#include "input/mouse.h"

Renderer* renderer_create(Renderer* renderer, int width, int height) {
	renderer->renderer = arenderer_create();
	renderer->width = width;
	renderer->height = height;
	renderer->wireframe = 0;
	renderer->backface_culling = 1;

	AAttachmentFormat attachments[] = { A_RGBA8, A_RED_INTEGER, A_DEPTH24STENCIL8 };
	framebuffer_create(&renderer->framebuffer, attachments, sizeof(attachments), width, height);

	if (!framebuffer_check_status(&renderer->framebuffer)) {
		log_error("Framebuffer is not complete!");
	}
	framebuffer_unbind(&renderer->framebuffer);

	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec2 a_pos;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"out vec2 v_tex_coord;\n"
		"void main() {\n"
		"	gl_Position = vec4(a_pos.xy, 0.0, 1.0);\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 v_tex_coord;\n"
		"uniform sampler2D u_texture;\n"
		"void main() {\n"
		"	FragColor = texture(u_texture, v_tex_coord);\n"
		"}\0";

	shader_create(&renderer->shader, src_vert, src_frag);
	mesh_create(&renderer->mesh);
	float vertices[] = {
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	};

	uint indices[] = {
		0, 2, 1,
		0, 3, 2
	};

	ADataType layout[] = { VEC2F, VEC2F };
	mesh_init_static(&renderer->mesh, vertices, sizeof(vertices), indices, sizeof(indices), layout, sizeof(layout), A_TRIANGLES);
	return renderer;
}

void renderer_delete(Renderer* renderer) {
	framebuffer_delete(&renderer->framebuffer);
	mesh_delete(&renderer->mesh);
	shader_delete(&renderer->shader);
	arenderer_delete(renderer->renderer);
}

void renderer_begin(Renderer* renderer) {
	framebuffer_bind(&renderer->framebuffer);
	arenderer_depth_test_set_enabled(renderer->renderer, 1);

	framebuffer_color_attachments_draw(&renderer->framebuffer);
	float background_color[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	framebuffer_color_attachment_clear_f(&renderer->framebuffer, 0, background_color);
	int entity_color[] = { -1 };
	framebuffer_color_attachment_clear_i(&renderer->framebuffer, 1, entity_color);

	arenderer_clear_buffer_depth(renderer->renderer);
	app.stats.draw_calls = 0;
}

void renderer_end(Renderer* renderer) {
	framebuffer_unbind(&renderer->framebuffer);
	arenderer_depth_test_set_enabled(renderer->renderer, 0);
	arenderer_clear_buffer_color(renderer->renderer);

	arenderer_polygon_mode_fill(renderer->renderer);

	mat4 view = mat4_identity();
	shader_bind(&renderer->shader, &view);

	framebuffer_attachment_bind(&renderer->framebuffer, 0);
	mesh_draw(&renderer->mesh);

	if (renderer->wireframe == 0) {
		arenderer_polygon_mode_fill(renderer->renderer);
	} else {
		arenderer_polygon_mode_line(renderer->renderer);
	}
}

void renderer_clear_depth(Renderer* renderer) {
	arenderer_clear_buffer_depth(renderer->renderer);
}

void renderer_toggle_backface_culling(Renderer* renderer) {
	renderer->backface_culling = 1 - renderer->backface_culling;
	arenderer_cull_face_set_enabled(renderer->renderer, renderer->backface_culling);
}

void renderer_toggle_fireframe(Renderer* renderer) {
	renderer->wireframe = 1 - renderer->wireframe;
	if (renderer->wireframe == 0) {
		arenderer_polygon_mode_fill(renderer->renderer);
	} else {
		arenderer_polygon_mode_line(renderer->renderer);
	}
}

int renderer_get_mouse_entity(Renderer* renderer) {
	framebuffer_bind(&renderer->framebuffer);
	int x = (int)get_mouse_x();
	int y = renderer->height - (int)get_mouse_y();
	int pixel = framebuffer_color_attachment_read_pixel(&renderer->framebuffer, 1, x, y);
	framebuffer_unbind(&renderer->framebuffer);
	return pixel;
}