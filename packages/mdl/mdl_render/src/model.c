#include "model.h"

#include "assets/gfx_material.h"
#include "assets/gfx_mesh.h"
#include "assets/gfx_shader.h"
#include "assets/gfx_texture.h"
#include "assets/image.h"

#include "mdl.h"

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

static void add_child(ModelObject *parent, ModelObject *child)
{
	ModelObject **target = &parent->child;
	while (*target != NULL) {
		target = &(*target)->next;
	}
	*target = child;
}

static ModelObject *load_object(ModelObject *objects, ModelObject *object, mdl_obj_t *obj, Renderer *renderer, Shader *shader)
{
	object->name = obj->name;
	if (obj->parent != -1) {
		add_child(&objects[obj->parent + 1], object);
	} else {
		add_child(&objects[0], object);
	}
	object->transformation = obj->transform;
	object->type	       = obj->type;

	if (object->type == 1) {
		object->mesh.name     = obj->mesh.name;
		object->mesh.material = obj->mesh.mat;

		mdl_vertex_uv_t *vertices;
		unsigned int vertices_cnt;
		void *indices;
		unsigned int indices_cnt;

		if (obj->mesh.flags & MDL_OBJ_MESH_UV) {
			vertices_cnt = obj->mesh.triangles * 3;
			vertices     = m_malloc(sizeof(mdl_vertex_uv_t) * vertices_cnt);
			indices	     = NULL;
			indices_cnt  = 0;

			object->vertices = vertices;
			object->vertices_size = sizeof(mdl_vertex_uv_t) * vertices_cnt;

			for (unsigned int i = 0; i < obj->mesh.triangles * 3; i++) {
				mdl_vertex_t *v = &obj->mesh.vertices[obj->mesh.indices[i]];

				vertices[i] = (mdl_vertex_uv_t){
					.px = v->px,
					.py = v->py,
					.pz = v->pz,
					.nx = v->nx,
					.ny = v->ny,
					.nz = v->nz,
					.u  = obj->mesh.uvs[i].u,
					.v  = obj->mesh.uvs[i].v,
				};
			}
		} else {
			vertices     = obj->mesh.vertices_uv;
			vertices_cnt = obj->mesh.vertices_cnt;
			indices	     = obj->mesh.indices;
			indices_cnt  = obj->mesh.triangles * 3;
		}

		AMeshData md = {
			.vertices.data = vertices,
			.vertices.size = vertices_cnt * sizeof(mdl_vertex_uv_t),
			.indices.data  = indices,
			.indices.size  = indices_cnt * sizeof(uint),
		};

		if (mesh_create(&object->mesh.mesh, renderer, shader, md, A_TRIANGLES) == NULL) {
			log_error("failed to create model mesh");
		}
	}

	return object;
}

static Texture *load_texture(Texture *texture, Image *image, Renderer *renderer, str_t *texture_name, const char *path, int type)
{
	if (texture_name->len == 0) {
		image_create(image, 1, 1, 4);
		if (image == NULL) {
			log_error("failed to create model image");
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
		str_create(&file, (uint)strlen(path) + texture_name->len + 1);
		str_add_cstr(&file, path, 0);

		Str name = {
			.cdata = texture_name->data,
			.count = texture_name->len,
		};
		str_add_str(&file, &name);

		image_load(image, file.data);
		if (image == NULL) {
			log_error("failed to load model image");
		}

		str_delete(&file);
	}
	return texture_create(texture, renderer, image, A_REPEAT, A_LINEAR);
}

static Material *load_material(mdl_mat_t *mat, Material *material, Image *diff_image, Texture *diff_texture, Image *spec_image, Texture *spec_texture, const char *path,
			       Renderer *renderer, Shader *shader)
{
	if (material_create(material, renderer, shader) == NULL) {
		log_error("failed to create model material");
		return material;
	}

	diff_texture = load_texture(diff_texture, diff_image, renderer, &mat->diff_tex, path, 0);
	if (diff_texture == NULL) {
		log_error("failed to create model texture");
		return material;
	}
	material_add_texture(material, diff_texture);

	spec_texture = load_texture(spec_texture, spec_image, renderer, &mat->spec_tex, path, 1);
	if (spec_texture == NULL) {
		log_error("failed to create model texture");
		return material;
	}
	material_add_texture(material, spec_texture);

	material_set_ps_value(material, 0, &mat->diff);
	material_set_ps_value(material, 1, &mat->spec);

	return material;
}

Model *model_load(Model *model, Renderer *renderer, const char *path, const char *filename, Shader *shader, bool flipUVS, bool print)
{
	*model = (Model){ 0 };

	Str file_path;
	str_create(&file_path, (uint)(strlen(path) + strlen(filename) + 1));
	str_add_cstr(&file_path, path, 0);
	str_add_cstr(&file_path, filename, 0);

	mdl_t *mdl = model->priv = m_malloc(sizeof(mdl_t));

	mdl_read(file_path.data, model->priv);

	model->objects_count = mdl->obj_cnt;
	model->objects = m_calloc(((size_t)model->objects_count + 1), sizeof(ModelObject));
	if (model->objects == NULL) {
		log_error("failed to allocate memory for objects\n");
		return model;
	}

	for (int i = 0; i < model->objects_count; i++) {
		load_object(model->objects, &model->objects[i + 1], &mdl->objs[i], renderer, shader);
	}

	model->materials_count = mdl->mat_cnt;
	model->materials       = m_malloc(model->materials_count * sizeof(Material));
	if (model->materials == NULL) {
		log_error("failed to allocate memory for materials\n");
		return model;
	}

	model->images_count = model->materials_count * 2;
	model->images	    = m_malloc(model->images_count * sizeof(Image));
	if (model->images == NULL) {
		log_error("failed to allocate memory for images\n");
		return model;
	}

	model->textures_count = model->images_count;
	model->textures	      = m_malloc(model->textures_count * sizeof(Texture));
	if (model->textures == NULL) {
		log_error("failed to allocate memory for textures\n");
		return model;
	}

	for (int i = 0; i < model->materials_count; i++) {
		load_material(&mdl->mats[i], &model->materials[i], &model->images[i * 2], &model->textures[i * 2], &model->images[i * 2 + 1], &model->textures[i * 2 + 1],
			      path, renderer, shader);
	}

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
			if (model->objects[i].vertices != NULL) {
				m_free(model->objects[i].vertices, model->objects[i].vertices_size);
			}
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

	mdl_free(model->priv);

	m_free(model->priv, sizeof(mdl_t));
}
