#pragma once

typedef struct ARenderer ARenderer;

ARenderer* arenderer_create();
void arenderer_delete(ARenderer* renderer);

void arenderer_clear(ARenderer* renderer, float red, float green, float blue, float alpha);
void arenderer_clear_depth(ARenderer* renderer);

void arenderer_cull_face_set_enabled(ARenderer* renderer, bool enabled);
void arenderer_polygon_mode_fill(ARenderer* renderer);
void arenderer_polygon_mode_line(ARenderer* renderer);