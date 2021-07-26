#include "pch.h"
#include "model_renderer.h"
#include "ecs/component/transform.h"
#include "ecs/component/mesh_component.h"

ModelRenderer* model_renderer_create(ModelRenderer* model_renderer, Assets* assets) {
	const char* src_vert =
		"#version 330 core\n"
		"layout (location = 0) in vec3 a_pos;\n"
		"layout (location = 1) in vec2 a_tex_coord;\n"
		"layout (std140) uniform Camera {\n"
		"	mat4 u_view_projection;\n"
		"};\n"
		"out vec2 v_tex_coord;\n"
		"uniform mat4 u_model;\n"
		"void main() {\n"
		"	gl_Position = u_view_projection * u_model * vec4(a_pos.xy, -a_pos.z, 1.0);\n"
		"	v_tex_coord = a_tex_coord;\n"
		"}\0";

	const char* src_frag =
		"#version 330 core\n"
		"layout (location = 0) out vec4 FragColor;\n"
		"layout (location = 1) out int color2;\n"
		"in vec2 v_tex_coord;\n"
		"uniform vec4 u_diffuse;\n"
		"uniform vec4 u_specular;\n"
		"uniform int u_entity;\n"
		"uniform sampler2D u_textures[16];\n"
		"void main() {\n"
		"	vec4 diffuse = u_diffuse * texture(u_textures[0], v_tex_coord);\n"
		"	vec4 specular = u_specular * texture(u_textures[1], v_tex_coord);\n"
		"	vec4 col = diffuse + specular;\n"
		"	FragColor = vec4(col.xyz, 1.0);\n"
		"	color2 = u_entity;\n"
		"}\0";

	model_renderer->shader = assets_shader_create(assets, "model_shader", src_vert, src_frag);
	if (model_renderer->shader == NULL) {
		log_error("Failed to create mdoel shader");
		return NULL;
	}

	return model_renderer;
}

void model_renderer_delete(ModelRenderer* model_renderer) {

}

void model_renderer_render(ModelRenderer* model_renderer, Ecs* ecs) {
	shader_bind(model_renderer->shader);

	QueryResult* qr = ecs_query(ecs, 2, C_TRANSFORM, C_MODEL);
	for (uint i = 0; i < qr->count; ++i) {
		Transform* transform = (Transform*)ecs_get(ecs, qr->list[i], C_TRANSFORM);
		Model* model = (Model*)ecs_get(ecs, qr->list[i], C_MODEL);

		shader_set_entity(model_renderer->shader, qr->list[i]);
		mat4 mat = transform_to_mat4(transform);
		model_draw(model, model_renderer->shader, mat);
	}
}