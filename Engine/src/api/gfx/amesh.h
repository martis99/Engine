#pragma once

typedef struct AMesh AMesh;

AMesh* amesh_create();
void amesh_delete(AMesh* mesh);

void amesh_init_static(AMesh* mesh, float* vertices, uint vertices_size, uint* indices, uint indices_size, uint* layout, uint layout_size);
void amesh_draw(AMesh* mesh);