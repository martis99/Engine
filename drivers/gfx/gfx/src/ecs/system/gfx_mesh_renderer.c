#include "gfx_mesh_renderer.h"
#include "ecs/component/transform.h"

#include "assets/gfx_material.h"
#include "assets/gfx_mesh.h"
#include "assets/gfx_shader.h"

#include "gfx_shader_creator.h"

MeshRenderer *mesh_renderer_create(MeshRenderer *mesh_renderer, Renderer *renderer)
{
	mesh_renderer->renderer = renderer;

	const char *src_vert = "VSOutput vs_main(VSInput vs_input) {\n"
			       "	VSOutput vs_output;\n"
			       "	vs_output.SV_Position = mul(vec4f(vs_input.Position, 1.0f), mul(Model, ViewProjection));\n"
			       "	vs_output.TexCoord = vs_input.TexCoord;\n"
			       "	return vs_output;\n"
			       "}\n"
			       "\0";

	const char *src_frag = "FSOutput fs_main(VSOutput fs_input) {\n"
			       "	FSOutput fs_output;\n"
			       "	fs_output.FragColor = Color * sample(Textures, 0, fs_input.TexCoord);\n"
			       "	fs_output.EntityId = Entity;\n"
			       "	return fs_output;\n"
			       "}\n"
			       "\0";

	AValue vs_in[] = {
		{ VEC3F, "Position" },
		{ VEC3F, "Normal" },
		{ VEC2F, "TexCoord" },
		{ VEC3F, "Color" },
	};

	AValue vs_out[] = {
		{ VEC4F, "SV_Position" },
		{ VEC2F, "TexCoord" },
	};

	AValue index[] = {
		{ VEC1UI, "" },
	};

	AValue global[] = {
		{ MAT4F, "ViewProjection" },
	};

	AValue vs[] = {
		{ MAT4F, "Model" },
	};

	AValue ps[] = {
		{ VEC4F, "Color" },
		{ VEC1F, "Entity" },
	};

	AValue output[] = {
		{ VEC4F, "FragColor" },
		{ VEC1F, "EntityId" },
	};

	ABufferDesc buffers[] = {
		{ A_BFR_VS_IN0, 0, vs_in, sizeof(vs_in), 0, "VSInput" },
		{ A_BFR_VS_OUT, 0, vs_out, sizeof(vs_out), 0, "VSOutput" },
		{ A_BFR_INDEX, 0, index, sizeof(index), 0, "" },
		{ A_BFR_GLOBAL, 0, global, sizeof(global), 0, "Camera" },
		{ A_BFR_VS, 1, vs, sizeof(vs), 0, "VSMaterial" },
		{ A_BFR_PS, 2, ps, sizeof(ps), 0, "PSMaterial" },
		{ A_BFR_PS_OUT, 0, output, sizeof(output), 0, "FSOutput" },
	};

	AShaderDesc shader_desc = {
		.buffers	= buffers,
		.buffers_size	= sizeof(buffers),
		.textures_count = 2,
		.texture_type	= VEC4F,
	};

	if (gfx_sc_create_shader(&renderer->shader_creator, &mesh_renderer->shader, renderer, src_vert, src_frag, shader_desc) == NULL) {
		log_error("failed to create mesh shader");
		return NULL;
	}

	return mesh_renderer;
}

void mesh_renderer_delete(MeshRenderer *mesh_renderer)
{
	shader_delete(&mesh_renderer->shader, mesh_renderer->renderer);
}

void mesh_renderer_begin(MeshRenderer *mesh_renderer)
{
	shader_bind(&mesh_renderer->shader, mesh_renderer->renderer);
}

void mesh_renderer_render(MeshRenderer *mesh_renderer, int id, Transform *transform, MeshComponent *mesh_component)
{
	mat4 model = transform_to_mat4(transform);
	material_set_vs_value(mesh_component->material, 0, &model);
	float idf = (float)id;
	material_set_ps_value(mesh_component->material, 1, &idf);
	material_upload(mesh_component->material, mesh_renderer->renderer);
	material_bind(mesh_component->material, mesh_renderer->renderer);
	mesh_draw(mesh_component->mesh, mesh_renderer->renderer, 0xFFFFFFFF);
}

void mesh_renderer_end(MeshRenderer *mesh_renderer)
{
}
