#pragma once

typedef struct ARenderer ARenderer;

ARenderer* arenderer_create();
void arenderer_delete(ARenderer* renderer);

void arenderer_clear_buffers(ARenderer* renderer);
void arenderer_clear_buffer_color(ARenderer* renderer);
void arenderer_clear_buffer_depth(ARenderer* renderer);
void arenderer_clear_color(ARenderer* renderer, float red, float green, float blue, float alpha);

void arenderer_depth_test_set_enabled(ARenderer* renderer, bool enabled);
void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled);
void arenderer_polygon_mode_fill(ARenderer* renderer);
void arenderer_polygon_mode_line(ARenderer* renderer);