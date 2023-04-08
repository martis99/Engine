#include "mdl.h"

#include "log.h"
#include "mem.h"
#include "print.h"

static void print_object(const mdl_obj_t *obj)
{
	printf("    Name: %.*s\n", obj->name.len, obj->name.data);
	printf("    Parent: %d\n", obj->parent);
	printf("    Transformation:\n");
	for (int i = 0; i < 4; i++) {
		printf("        %6.2f %6.2f %6.2f %6.2f\n", obj->transform.m[i][0], obj->transform.m[i][1], obj->transform.m[i][2], obj->transform.m[i][3]);
	}
	printf("    Type: %d\n", obj->type);

	if (obj->type == 1) {
		printf("    Mesh:\n");
		printf("        Name: %.*s\n", obj->mesh.name.len, obj->mesh.name.data);
		printf("        Material: %d\n", obj->mesh.mat);
		printf("        Flags: %s\n", obj->mesh.flags & MDL_OBJ_MESH_UV ? "UV" : "");
		printf("        Vertices[%d] = [\n", obj->mesh.vertices_cnt);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
		const unsigned int verts = MIN(2, obj->mesh.vertices_cnt);
		for (unsigned int i = 0; i < verts; i++) {
			if (obj->mesh.flags & MDL_OBJ_MESH_UV) {
				const mdl_vertex_t *v = &obj->mesh.vertices[i];
				if (i > 0) {
					printf("        },\n");
				}
				printf("        [%d] = {\n", i);
				printf("            Position: %6.2f, %6.2f, %6.2f\n", v->px, v->py, v->pz);
				printf("            Normal: %6.2f, %6.2f, %6.2f\n", v->nx, v->ny, v->nz);
			} else {
				const mdl_vertex_uv_t *v = &obj->mesh.vertices_uv[i];
				if (i > 0) {
					printf("        },\n");
				}
				printf("        [%d] = {\n", i);
				printf("            Position: %6.2f, %6.2f, %6.2f\n", v->px, v->py, v->pz);
				printf("            Normal: %6.2f, %6.2f, %6.2f\n", v->nx, v->ny, v->nz);
				printf("            UV: %6.2f, %6.2f\n", v->u, v->v);
			}
		}

		if (verts <= 2) {
			printf("        }, {\n");
			printf("            ...\n");
		}

		printf("        }\n");
		printf("        ]\n");
		printf("        Indices[%d] = [\n", obj->mesh.triangles * 3);

		const unsigned int inds = MIN(6, obj->mesh.triangles);
		for (unsigned int i = 0; i < inds; i++) {
			printf("            %d, %d, %d\n", obj->mesh.indices[i * 3 + 0], obj->mesh.indices[i * 3 + 1], obj->mesh.indices[i * 3 + 2]);
		}

		if (inds <= 6) {
			printf("            ...\n");
		}

		printf("        ]\n");

		if (obj->mesh.flags & MDL_OBJ_MESH_UV) {
			printf("        UVs[%d] = [\n", obj->mesh.triangles * 3);

			for (unsigned int i = 0; i < inds; i++) {
				// clang-format off
			printf("            (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", 
				obj->mesh.uvs[i * 3 + 0].u, obj->mesh.uvs[i * 3 + 0].v,
				obj->mesh.uvs[i * 3 + 1].u, obj->mesh.uvs[i * 3 + 1].v,
				obj->mesh.uvs[i * 3 + 2].u, obj->mesh.uvs[i * 3 + 2].v
			);
				// clang-format on
			}

			if (inds <= 6) {
				printf("            ...\n");
			}

			printf("        ]\n");
		}
	}
}

static void print_material(const mdl_mat_t *mat)
{
	printf("    Name: %.*s\n", mat->name.len, mat->name.data);
	printf("    Diffuse: %6.2f, %6.2f, %6.2f, %6.2f\n", mat->diff.r, mat->diff.g, mat->diff.b, mat->diff.a);
	printf("    DiffuseTexture: %.*s\n", mat->diff_tex.len, mat->diff_tex.data);
	printf("    Specular: %6.2f, %6.2f, %6.2f, %6.2f\n", mat->spec.r, mat->spec.g, mat->spec.b, mat->spec.a);
	printf("    SpecularTexture: %.*s\n", mat->spec_tex.len, mat->spec_tex.data);
	printf("    Roughness: %6.2f\n", mat->roughness);
	printf("    Metallic: %6.2f\n", mat->metallic);
}

static void print_mdl(const mdl_t *mdl)
{
	printf("Objects[%d] = [\n", mdl->obj_cnt);

	for (unsigned int i = 0; i < mdl->obj_cnt; i++) {
		if (i > 0) {
			printf("},\n");
		}
		printf("[%d] = {\n", i);
		print_object(&mdl->objs[i]);
	}

	printf("}\n");
	printf("]\n");

	printf("Materials[%d] = [\n", mdl->mat_cnt);

	for (unsigned int i = 0; i < mdl->mat_cnt; i++) {
		if (i > 0) {
			printf("},\n");
		}
		printf("[%d] = {\n", i);
		print_material(&mdl->mats[i]);
	}

	printf("}\n");
	printf("]\n");

	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		p_fprintf(stderr, "Usage: %s <path>\n", argv[0]);
		return 1;
	}

	m_stats_t m_stats = { 0 };
	m_init(&m_stats);

	log_t s_log = { 0 };
	log_init(&s_log);

	mdl_t mdl = { 0 };

	int ret = 0;

	mdl_read(argv[1], &mdl);

	printf("File size: %zu KB (%zu B)\n\n", mdl.dsize / 1024, mdl.dsize);

	print_mdl(&mdl);

	mdl_free(&mdl);

	m_print(stdout);

	return ret;
}
