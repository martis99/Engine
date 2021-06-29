#pragma once

typedef struct ARenderer ARenderer;

ARenderer* arenderer_create();
void arenderer_delete(ARenderer* renderer);

void arenderer_clear(ARenderer* renderer, float red, float green, float blue, float alpha);