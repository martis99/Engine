#include "pch.h"
#include "model.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "image.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"

Model* model_create(Model* model) {
	return model;
}

static void delete_mesh(ModelMesh* mesh) {
	mesh_delete(mesh->mesh);
	m_free(mesh->mesh, sizeof(Mesh));
}

static void delete_node(ModelNode* node) {
	arr_delete(&node->nodes, delete_node);
	arr_delete(&node->meshes, delete_mesh);
}

void model_delete(Model* model) {
	delete_node(&model->node);
	arr_delete(&model->images, image_delete);
	arr_delete(&model->textures, texture_delete);
	arr_delete(&model->materials, material_delete);
}

static void node_draw(Model* model, Renderer* renderer, Shader* shader, ModelNode* node, mat4 transformation) {
	mat4 trans = mat4_mul(node->transformation, transformation);
	for (uint i = 0; i < node->meshes.count; i++) {
		ModelMesh* mesh = arr_get(&node->meshes, i);
		if (mesh->material >= 0) {
			material_set_value(arr_get(&model->materials, mesh->material), 0, &trans);
			material_bind(arr_get(&model->materials, mesh->material), renderer, 1);
		}
		mesh_draw_elements(mesh->mesh, renderer);
	}

	for (uint i = 0; i < node->nodes.count; i++) {
		node_draw(model, renderer, shader, arr_get(&node->nodes, i), trans);
	}
}

void model_draw(Model* model, Renderer* renderer, Shader* shader, mat4 transformation) {
	node_draw(model, renderer, shader, &model->node, transformation);
}

static void print_material_name(const struct aiMaterial* material, int depth, bool print) {
	if (print) {
		for (int i = 0; i < depth; i++) {
			printf("   ");
		}
		struct aiString name;
		aiGetMaterialString(material, AI_MATKEY_NAME, &name);
		printf("Material: %s\n", name.data);
	}
}

static void process_mesh(ModelMesh* mesh, Renderer* renderer, Shader* shader, const struct aiMesh* ai_mesh, const struct aiScene* ai_scene, int depth, bool print) {
	if (print) {
		for (int i = 0; i < depth; i++) {
			printf("   ");
		}
		printf("Mesh: %s\n", ai_mesh->mName.data);
	}

	typedef struct Vertex {
		vec3 pos;
		vec2 tex;
	} Vertex;

	Vertex* vertices = m_malloc(sizeof(Vertex) * ai_mesh->mNumVertices);
	uint* indices = m_malloc(sizeof(uint) * ai_mesh->mNumFaces * 3);

	for (uint i = 0; i < ai_mesh->mNumVertices; i++) {
		vertices[i].pos.x = ai_mesh->mVertices[i].x;
		vertices[i].pos.y = ai_mesh->mVertices[i].y;
		vertices[i].pos.z = ai_mesh->mVertices[i].z;

		if (ai_mesh->mTextureCoords[0]) {
			vertices[i].tex.x = ai_mesh->mTextureCoords[0][i].x;
			vertices[i].tex.y = ai_mesh->mTextureCoords[0][i].y;
		} else {
			vertices[i].tex.x = 0;
			vertices[i].tex.y = 0;
		}
	}

	int index = 0;
	for (uint i = 0; i < ai_mesh->mNumFaces; i++) {
		struct aiFace face = ai_mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++) {
			indices[index++] = face.mIndices[j];
		}
	}

	print_material_name(ai_scene->mMaterials[ai_mesh->mMaterialIndex], depth + 1, print);
	mesh->material = ai_mesh->mMaterialIndex;

	mesh->mesh = m_malloc(sizeof(Mesh));
	mesh_create(mesh->mesh);
	mesh_init_static(mesh->mesh, renderer, shader, vertices, sizeof(Vertex) * ai_mesh->mNumVertices, 5 * sizeof(float), indices, sizeof(uint) * index, sizeof(uint), A_TRIANGLES);

	m_free(vertices, sizeof(Vertex) * ai_mesh->mNumVertices);
	m_free(indices, sizeof(uint) * ai_mesh->mNumFaces * 3);
}

static void process_node(ModelNode* node, Renderer* renderer, Shader* shader, const struct aiNode* ai_node, const struct aiScene* ai_scene, int depth, bool print) {
	if (print == 1) {
		for (int i = 0; i < depth; i++) {
			printf("   ");
		}
		printf("Node: %s\n", ai_node->mName.data);
	}

	arr_create(&node->meshes, sizeof(ModelMesh), ai_node->mNumMeshes);
	for (uint i = 0; i < ai_node->mNumMeshes; i++) {
		struct aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
		process_mesh(arr_add(&node->meshes), renderer, shader, ai_mesh, ai_scene, depth + 1, print);
	}

	arr_create(&node->nodes, sizeof(ModelNode), ai_node->mNumChildren);
	for (uint i = 0; i < ai_node->mNumChildren; i++) {
		process_node(arr_add(&node->nodes), renderer, shader, ai_node->mChildren[i], ai_scene, depth + 1, print);
	}

	memcpy(&node->transformation, &ai_node->mTransformation, sizeof(node->transformation));
	node->transformation = mat4_invert(node->transformation);
}

static char* merge(const char* left, const char* right) {
	size_t size = strlen(left) + strlen(right) + 1;
	char* res = m_malloc(size);
	strcpy_s(res, size, left);
	strcat_s(res, size, right);
	return res;
}

static void print_texture_type(enum aiTextureType type, char* file, int depth, bool print) {
	if (print == 1) {
		for (int i = 0; i < depth; i++) {
			printf("   ");
		}
		switch (type) {
		case aiTextureType_DIFFUSE:
			printf("Diffuse texture: %s\n", file);
			break;
		case aiTextureType_SPECULAR:
			printf("Specular texture: %s\n", file);
			break;
		}
	}
}

static void process_textures(Model* model, Renderer* renderer, Material* material, const char* path, const struct aiMaterial* ai_material, enum aiTextureType type, int depth, bool print) {
	bool found = 0;
	for (uint i = 0; i < aiGetMaterialTextureCount(ai_material, type); i++) {
		struct aiString str;
		aiGetMaterialTexture(ai_material, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
		char* file = merge(path, str.data);
		print_texture_type(type, file, depth, print);
		Image* image = image_load(arr_add(&model->images), file);
		Texture* texture = texture_create(arr_add(&model->textures), renderer, image, A_REPEAT, A_LINEAR);
		material_add_texture(material, texture);

		m_free(file, strlen(file) + 1);
		found = 1;
	}

	if (found == 0) {
		Image* image = image_create(arr_add(&model->images), 1, 1, 4);
		uint data;
		if (type == aiTextureType_SPECULAR) {
			print_texture_type(type, "black", depth, print);
			data = (uint)0xff000000;
		} else {
			print_texture_type(type, "white", depth, print);
			data = (uint)0xffffffff;
		}
		image_set_data(image, (unsigned char*)&data);
		Texture* texture = texture_create(arr_add(&model->textures), renderer, image, A_CLAMP_TO_EDGE, A_NEAREST);
		material_add_texture(material, texture);
	}
}

static void process_material(Model* model, Renderer* renderer, Shader* shader, Material* material, const char* path, const struct aiMaterial* ai_material, int depth, bool print) {
	print_material_name(ai_material, depth, print);

	material_create(material, renderer, shader);

	process_textures(model, renderer, material, path, ai_material, aiTextureType_DIFFUSE, depth + 1, print);
	process_textures(model, renderer, material, path, ai_material, aiTextureType_SPECULAR, depth + 1, print);

	struct aiColor4D diffuseColor;
	aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
	vec4 diff = (vec4){ diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };
	material_set_value(material, 2, &diff);
	if (print == 1) {
		for (int i = 0; i < depth + 1; i++) {
			printf("   ");
		}
		printf("Diffuse Color: (%f, %f, %f, %f)\n", diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
	}

	struct aiColor4D specularColor;
	aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
	vec4 spec = (vec4){ specularColor.r, specularColor.g, specularColor.b, specularColor.a };
	material_set_value(material, 3, &spec);
	if (print == 1) {
		for (int i = 0; i < depth + 1; i++) {
			printf("   ");
		}
		printf("Specular Color: (%f, %f, %f, %f)\n", specularColor.r, specularColor.g, specularColor.b, specularColor.a);
	}
}

Model* model_load(Model* model, Renderer* renderer, const char* path, const char* filename, Shader* shader, bool flipUVs, bool print) {
	char* file = merge(path, filename);
	if (print == 1) {
		printf("%s\n", file);
	}
	const struct aiScene* ai_scene = aiImportFile(file, aiProcess_Triangulate | aiProcess_FlipUVs * flipUVs);
	arr_create(&model->materials, sizeof(Material), ai_scene->mNumMaterials);
	arr_create(&model->images, sizeof(Image), ai_scene->mNumMaterials * 2);
	arr_create(&model->textures, sizeof(Texture), ai_scene->mNumMaterials * 2);

	for (uint i = 0; i < ai_scene->mNumMaterials; i++) {
		process_material(model, renderer, shader, arr_add(&model->materials), path, ai_scene->mMaterials[i], 1, print);
	}

	process_node(&model->node, renderer, shader, ai_scene->mRootNode, ai_scene, 1, print);

	m_free(file, strlen(file) + 1);

	if (print == 1) {
		printf("\n");
	}

	return model;
}