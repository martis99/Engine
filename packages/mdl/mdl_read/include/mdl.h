#ifndef MDL_H
#define MDL_H

#include "c_math.h"
#include "str.h"

typedef struct mdl_vertex_s {
	float px, py, pz;
	float nx, ny, nz;
} mdl_vertex_t;

typedef struct mdl_vertex_uv_s {
	float px, py, pz;
	float nx, ny, nz;
	float u, v;
} mdl_vertex_uv_t;

typedef unsigned int mdl_index_t;

typedef struct mdl_uv_s {
	float u, v;
} mdl_uv_t;

typedef enum mdl_obj_mesh_flag_e {
	MDL_OBJ_MESH_UV = 1 << 0,
} mdl_obj_mesh_flag_t;

typedef struct mdl_obj_mesh_s {
	str_t name;
	unsigned int mat;
	unsigned int flags;
	unsigned int vertices_cnt;
	union {
		mdl_vertex_t *vertices;
		mdl_vertex_uv_t *vertices_uv;
	};
	unsigned int triangles;
	mdl_index_t *indices;
	mdl_uv_t *uvs;
} mdl_obj_mesh_t;

typedef struct mdl_obj_s {
	str_t name;
	unsigned int parent;
	mat4 transform;
	unsigned char type;
	mdl_obj_mesh_t mesh;
} mdl_obj_t;

typedef struct mdl_mat_s {
	str_t name;
	vec4 diff;
	str_t diff_tex;
	vec4 spec;
	str_t spec_tex;
	float roughness;
	float metallic;
} mdl_mat_t;

typedef struct mdl_s {
	unsigned char *data;
	size_t dsize;
	unsigned int obj_cnt;
	mdl_obj_t *objs;
	unsigned int mat_cnt;
	mdl_mat_t *mats;

} mdl_t;

mdl_t *mdl_read(const char *path, mdl_t *mdl);
void mdl_free(mdl_t *mdl);

#endif
