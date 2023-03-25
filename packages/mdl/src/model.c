#include "model.h"

#include "assets/gfx_material.h"
#include "assets/gfx_mesh.h"
#include "assets/gfx_shader.h"
#include "assets/gfx_texture.h"
#include "assets/image.h"

#include "math/maths.h"

#include "file.h"

#include <stdlib.h>
#include <string.h>

Model *model_create(Model *model)
{
	return model;
}

static void object_draw(Model *model, Renderer *renderer, Shader *shader, ModelObject *object, mat4 transformation, int entity)
{
	ModelObject *next = object;
	while (next != NULL) {
		mat4 trans	   = mat4_mul(transformation, next->transformation);
		ObjectMesh *mesh   = &next->mesh;
		Material *material = &model->materials[mesh->material];
		material_set_vs_value(material, 0, &trans);
		float entityf = (float)entity;
		material_set_ps_value(material, 2, &entityf);
		material_upload(material, renderer);
		material_bind(material, renderer);
		mesh_draw(&mesh->mesh, renderer, 0xFFFFFFFF);

		object_draw(model, renderer, shader, next->child, trans, entity);
		next = next->next;
	}
}

void model_draw(Model *model, Renderer *renderer, Shader *shader, mat4 transformation, int entity)
{
	if (model->objects_count > 0) {
		object_draw(model, renderer, shader, model->objects[0].child, transformation, entity);
	}
}

typedef struct BData {
	unsigned char *data;
	unsigned char *cur;
} BData;

unsigned char read_byte(BData *data)
{
	unsigned char value = *data->cur;
	data->cur += sizeof(unsigned char);
	return value;
}

static int read_int(BData *data)
{
	int value = *(int *)data->cur;
	data->cur += sizeof(int);
	return value;
}

static float read_float(BData *data)
{
	float value = *(float *)data->cur;
	data->cur += sizeof(float);
	return value;
}

static vec4 read_vec4(BData *data)
{
	return (vec4){
		read_float(data),
		read_float(data),
		read_float(data),
		read_float(data),
	};
}

static void read_mat4(BData *data, mat4 *mat)
{
	for (int i = 0; i < 16; i++) {
		mat->a[i] = read_float(data);
	}
}

static Str read_str(BData *data)
{
	int length = read_int(data);
	Str str;
	str_create_cstrn(&str, data->cur, length);
	data->cur += length;
	return str;
}

static void add_child(ModelObject *parent, ModelObject *child)
{
	ModelObject **target = &parent->child;
	while (*target != NULL) {
		target = &(*target)->next;
	}
	*target = child;
}

static ModelObject *read_object(ModelObject *objects, ModelObject *object, BData *data, Renderer *renderer, Shader *shader)
{
	object->name = read_str(data);
	int parent   = read_int(data);
	if (parent != -1) {
		add_child(&objects[parent + 1], object);
	} else {
		add_child(&objects[0], object);
	}
	read_mat4(data, &object->transformation);
	object->type = read_byte(data);
	if (object->type == 1) {
		object->mesh.name     = read_str(data);
		object->mesh.material = read_int(data);
		int vertices_count    = read_int(data);
		float *vertices	      = (float *)data->cur;
		data->cur += vertices_count * sizeof(float) * 8;
		int indices_count = read_int(data);
		uint *indices	  = (uint *)data->cur;
		data->cur += indices_count * sizeof(uint) * 3;

		AMeshData md = {
			.vertices.data = vertices,
			.vertices.size = vertices_count * sizeof(float) * 8,
			.indices.data  = indices,
			.indices.size  = indices_count * sizeof(uint) * 3,
		};

		if (mesh_create(&object->mesh.mesh, renderer, shader, md, A_TRIANGLES) == NULL) {
			log_msg(renderer->log, "Failed to create model mesh");
			return NULL;
		}
	}

	return object;
}

static Texture *read_texture(Texture *texture, Image *image, Renderer *renderer, Str texture_name, const char *path, int type)
{
	if (texture_name.count == 0) {
		image_create(image, 1, 1, 4);
		if (image == NULL) {
			log_msg(renderer->log, "Failed to create model image");
			return NULL;
		}
		uint data;
		if (type == 0) {
			data = (uint)0xffffffff;
		} else if (type == 1) {
			data = (uint)0xff000000;
		} else {
			data = (uint)0xff000000;
		}
		image_set_data(image, (unsigned char *)&data);
	} else {
		Str file;
		str_create(&file, (uint)strlen(path) + texture_name.count + 1);
		str_add_cstr(&file, path, 0);
		str_add_str(&file, &texture_name);

		image_load(image, file.data);
		if (image == NULL) {
			log_msg(renderer->log, "Failed to load model image");
			return NULL;
		}

		str_delete(&file);
	}
	return texture_create(texture, renderer, image, A_REPEAT, A_LINEAR);
}

static Material *read_material(BData *data, Material *material, Image *diff_image, Texture *diff_texture, Image *spec_image, Texture *spec_texture, const char *path,
			       Renderer *renderer, Shader *shader)
{
	read_str(data);
	vec4 diffuse	     = read_vec4(data);
	Str diffuse_texture  = read_str(data);
	vec4 specular	     = read_vec4(data);
	Str specular_texture = read_str(data);
	float roughness	     = read_float(data);
	float metallic	     = read_float(data);

	if (material_create(material, renderer, shader) == NULL) {
		log_msg(renderer->log, "Failed to create model material");
		return NULL;
	}

	diff_texture = read_texture(diff_texture, diff_image, renderer, diffuse_texture, path, 0);
	if (diff_texture == NULL) {
		log_msg(renderer->log, "Failed to create model texture");
		return NULL;
	}
	material_add_texture(material, diff_texture);

	spec_texture = read_texture(spec_texture, spec_image, renderer, specular_texture, path, 1);
	if (spec_texture == NULL) {
		log_msg(renderer->log, "Failed to create model texture");
		return NULL;
	}
	material_add_texture(material, spec_texture);

	material_set_ps_value(material, 0, &diffuse);
	material_set_ps_value(material, 1, &specular);

	return material;
}

Model *model_load(Model *model, Renderer *renderer, const char *path, const char *filename, Shader *shader, bool flipUVS, bool print)
{
	Str file_path;
	str_create(&file_path, (uint)(strlen(path) + strlen(filename) + 1));
	str_add_cstr(&file_path, path, 0);
	str_add_cstr(&file_path, filename, 0);

	FILE *file = file_open(file_path.data, "rb", 1);
	if (file == NULL) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	BData data = {
		.data = m_malloc(fsize),
		.cur  = data.data,
	};

	if (data.data == NULL) {
		return 0;
	}

	fread(data.data, fsize, 1, file);
	fclose(file);

	model->objects_count = read_int(&data);

	model->objects = m_calloc((size_t)model->objects_count + 1, sizeof(ModelObject));
	if (model->objects == NULL) {
		return NULL;
	}

	for (int i = 0; i < model->objects_count; i++) {
		read_object(model->objects, &model->objects[i + 1], &data, renderer, shader);
	}

	model->materials_count = read_int(&data);
	model->materials       = m_calloc(model->materials_count, sizeof(Material));
	if (model->materials == NULL) {
		return NULL;
	}

	model->images_count = model->materials_count * 2;
	model->images	    = m_calloc(model->images_count, sizeof(Image));
	if (model->images == NULL) {
		return NULL;
	}

	model->textures_count = model->images_count;
	model->textures	      = m_calloc(model->textures_count, sizeof(Texture));
	if (model->textures == NULL) {
		return NULL;
	}

	for (int i = 0; i < model->materials_count; i++) {
		read_material(&data, &model->materials[i], &model->images[i * 2], &model->textures[i * 2], &model->images[i * 2 + 1], &model->textures[i * 2 + 1], path,
			      renderer, shader);
	}

	m_free(data.data, fsize);
	str_delete(&file_path);
	return model;
}

void model_delete(Model *model, Renderer *renderer)
{
	for (int i = 0; i < model->materials_count; i++) {
		material_delete(&model->materials[i], renderer);
	}
	m_free(model->materials, model->materials_count * sizeof(Material));

	for (int i = 0; i < model->objects_count + 1; i++) {
		if (model->objects[i].type == 1) {
			mesh_delete(&model->objects[i].mesh.mesh, renderer);
		}
	}
	m_free(model->objects, ((size_t)model->objects_count + 1) * sizeof(ModelObject));

	for (int i = 0; i < model->textures_count; i++) {
		texture_delete(&model->textures[i], renderer);
	}
	m_free(model->textures, model->textures_count * sizeof(Texture));

	for (int i = 0; i < model->images_count; i++) {
		image_delete(&model->images[i]);
	}
	m_free(model->images, model->images_count * sizeof(Image));
}
