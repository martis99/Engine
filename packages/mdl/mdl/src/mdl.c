#include "mdl.h"

#include "file.h"
#include "log.h"
#include "mem.h"
#include "print.h"
#include "str.h"

typedef struct data_s {
	unsigned char *data;
	const unsigned char *cur;
	size_t size;
} data_t;

static int read_data(const char *path, data_t *data)
{
	if (!file_exists(path)) {
		log_error("file '%s' does not exists\n", path);
		return 1;
	}

	FILE *file = file_open(path, "rb");
	if (file == NULL) {
		log_error("failed to open file '%s'\n", path);
		return 1;
	}

	data->size = file_size(file);
	data->data = m_malloc(data->size);
	data->cur  = data->data;

	if (data->data == NULL) {
		log_error("failed to allocate memory for mdl data\n");
		file_close(file);
		return 1;
	}

	if (file_read(file, data->size, data->data, data->size) == 0) {
		log_error("failed to read mdl file\n");
		file_close(file);
		return 1;
	}

	file_close(file);

	return 0;
}

unsigned char read_byte(data_t *data)
{
	const unsigned char value = *data->cur;
	data->cur += sizeof(unsigned char);
	return value;
}

static int read_int(data_t *data)
{
	const int value = *(int *)data->cur;
	data->cur += sizeof(int);
	return value;
}

static float read_float(data_t *data)
{
	const float value = *(float *)data->cur;
	data->cur += sizeof(float);
	return value;
}

static vec4 read_vec4(data_t *data)
{
	return (vec4){
		read_float(data),
		read_float(data),
		read_float(data),
		read_float(data),
	};
}

static void read_mat4(data_t *data, mat4 *mat)
{
	for (int i = 0; i < 16; i++) {
		mat->a[i] = read_float(data);
	}
}

static str_t read_str(data_t *data)
{
	const int len = read_int(data);
	str_t str     = {
		    .data = data->cur,
		    .len  = len,
	};
	data->cur += len;
	return str;
}

static int read_object(data_t *data, mdl_obj_t *obj)
{
	if ((size_t)(data->cur - data->data) > data->size) {
		log_error("failed to parse mdl file");
		return 1;
	}

	obj->name   = read_str(data);
	obj->parent = read_int(data);

	read_mat4(data, &obj->transform);

	obj->type = read_byte(data);

	if (obj->type == 1) {
		obj->mesh.name	       = read_str(data);
		obj->mesh.mat	       = read_int(data);
		obj->mesh.flags	       = read_int(data);
		obj->mesh.vertices_cnt = read_int(data);

		if (obj->mesh.flags & MDL_OBJ_MESH_UV) {
			obj->mesh.vertices = (mdl_vertex_t *)data->cur;
			data->cur += obj->mesh.vertices_cnt * sizeof(mdl_vertex_t);
		} else {
			obj->mesh.vertices_uv = (mdl_vertex_uv_t *)data->cur;
			data->cur += obj->mesh.vertices_cnt * sizeof(mdl_vertex_uv_t);
		}

		if ((size_t)(data->cur - data->data) > data->size) {
			log_error("failed to parse mdl file");
			return 1;
		}

		obj->mesh.triangles = read_int(data);
		obj->mesh.indices   = (mdl_index_t *)data->cur;
		data->cur += obj->mesh.triangles * sizeof(mdl_index_t) * 3;

		if (obj->mesh.flags & MDL_OBJ_MESH_UV) {
			obj->mesh.uvs = (mdl_uv_t *)data->cur;
			data->cur += obj->mesh.triangles * sizeof(mdl_uv_t) * 3;
		} else {
			obj->mesh.uvs = 0;
		}
	}

	return 0;
}

static int read_material(data_t *data, mdl_mat_t *mat)
{
	if ((size_t)(data->cur - data->data) > data->size) {
		log_error("failed to parse mdl file");
		return 1;
	}
	mat->name      = read_str(data);
	mat->diff      = read_vec4(data);
	mat->diff_tex  = read_str(data);
	mat->spec      = read_vec4(data);
	mat->spec_tex  = read_str(data);
	mat->roughness = read_float(data);
	mat->metallic  = read_float(data);

	return 0;
}

mdl_t *mdl_read(const char *path, mdl_t *mdl)
{
	data_t data = { 0 };

	*mdl = (mdl_t){ 0 };

	if (read_data(path, &data)) {
		return mdl;
	}

	mdl->data  = data.data;
	mdl->dsize = data.size;

	mdl->obj_cnt = read_int(&data);
	mdl->objs    = m_malloc(sizeof(mdl_obj_t) * mdl->obj_cnt);

	for (unsigned int i = 0; i < mdl->obj_cnt; i++) {
		if (read_object(&data, &mdl->objs[i])) {
			return mdl;
		}
	}

	mdl->mat_cnt = read_int(&data);
	mdl->mats    = m_malloc(sizeof(mdl_mat_t) * mdl->mat_cnt);

	for (unsigned int i = 0; i < mdl->mat_cnt; i++) {
		if (read_material(&data, &mdl->mats[i])) {
			return mdl;
		}
	}

	if (data.cur - data.data != data.size) {
		log_error("failed to parse mdl file");
	}

	return mdl;
}

void mdl_free(mdl_t *mdl)
{
	m_free(mdl->mats, sizeof(mdl_mat_t) * mdl->mat_cnt);
	m_free(mdl->objs, sizeof(mdl_obj_t) * mdl->obj_cnt);
	m_free(mdl->data, mdl->dsize);
}
