#include "file.h"
#include "mem.h"
#include "print.h"
#include "str.h"

typedef struct data_s {
	unsigned char *data;
	const unsigned char *cur;
	size_t size;
} data_t;

typedef union vec4 {
	struct {
		float x, y, z, w;
	};
	struct {
		float r, g, b, a;
	};
} vec4;

typedef union mat4 {
	float m[4][4];
	float a[16];
	struct {
		float a11, a12, a13, a14;
		float a21, a22, a23, a24;
		float a31, a32, a33, a34;
		float a41, a42, a43, a44;
	};
} mat4;

static void log_err(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	p_vfprintf(stderr, fmt, args);
	va_end(args);
}

static void log_msg(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	p_vfprintf(stderr, fmt, args);
	va_end(args);
}

static int read_data(const char *path, data_t *data)
{
	if (!file_exists(path)) {
		log_err("File '%s' does not exists\n", path);
		return 1;
	}

	FILE *file = file_open(path, "rb");
	if (file == NULL) {
		log_err("Failed to open file '%s'\n", path);
		return 1;
	}

	data->size = file_size(file);
	data->data = m_malloc(data->size);
	data->cur  = data->data;

	if (data->data == NULL) {
		log_err("Failed to allocate memory for data\n");
		file_close(file);
		return 1;
	}

	if (file_read(file, data->size, data->data, data->size)) {
		log_err("Failed to read file\n");
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

static int read_object(data_t *data)
{
	const str_t name	  = read_str(data);
	const unsigned int parent = read_int(data);

	mat4 transformation = { 0 };
	read_mat4(data, &transformation);

	const unsigned char type = read_byte(data);

	log_msg("    Name: %.*s\n", name.len, name.data);
	log_msg("    Parent: %d\n", parent);
	log_msg("    Transformation:\n");
	for (int i = 0; i < 4; i++) {
		log_msg("        %6.2f %6.2f %6.2f %6.2f\n", transformation.m[i][0], transformation.m[i][1], transformation.m[i][2], transformation.m[i][3]);
	}
	log_msg("    Type: %d\n", type);

	if (type == 1) {
		log_msg("    Mesh:\n");

		const str_t mesh_name		 = read_str(data);
		const unsigned int mesh_material = read_int(data);
		const unsigned int vertices_cnt	 = read_int(data);

		typedef struct vertex_s {
			float px, py, pz;
			float nx, ny, nz;
			float tu, tv;
		} vertex_t;

		const vertex_t *vertices = (vertex_t *)data->cur;
		data->cur += vertices_cnt * sizeof(float) * 8;
		const unsigned int indices_cnt = read_int(data);
		const unsigned int *indices    = (unsigned int *)data->cur;
		data->cur += indices_cnt * sizeof(unsigned int) * 3;

		log_msg("        Name: %.*s\n", mesh_name.len, mesh_name.data);
		log_msg("        Material: %d\n", mesh_material);
		log_msg("        Vertices[%d] = [\n", vertices_cnt);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
		const unsigned int verts = MIN(2, vertices_cnt);
		for (unsigned int i = 0; i < verts; i++) {
			const vertex_t *v = &vertices[i];
			if (i > 0) {
				log_msg("        },\n");
			}
			log_msg("        [%d] = {\n", i);
			log_msg("            Position: %6.2f, %6.2f, %6.2f\n", v->px, v->py, v->pz);
			log_msg("            Normal: %6.2f, %6.2f, %6.2f\n", v->nx, v->ny, v->nz);
			log_msg("            TexCoord: %6.2f, %6.2f\n", v->tu, v->tv);
		}

		if (verts <= 2) {
			log_msg("        }, {\n");
			log_msg("            ...\n");
		}

		log_msg("        }\n");
		log_msg("        ]\n");

		log_msg("        Indices[%d] = [\n", indices_cnt);

		const unsigned int inds = MIN(6, indices_cnt);
		for (unsigned int i = 0; i < inds; i++) {
			log_msg("            %d, %d, %d\n", indices[i * 3 + 0], indices[i * 3 + 1], indices[i * 3 + 2]);
		}

		if (inds <= 6) {
			log_msg("            ...\n");
		}

		log_msg("        ]\n");
	}

	return 0;
}

static int read_material(data_t *data)
{
	str_t name	       = read_str(data);
	vec4 diffuse	       = read_vec4(data);
	str_t diffuse_texture  = read_str(data);
	vec4 specular	       = read_vec4(data);
	str_t specular_texture = read_str(data);
	float roughness	       = read_float(data);
	float metallic	       = read_float(data);

	log_msg("    Name: %.*s\n", name.len, name.data);
	log_msg("    Diffuse: %6.2f, %6.2f, %6.2f, %6.2f\n", diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	log_msg("    DiffuseTexture: %.*s\n", diffuse_texture.len, diffuse_texture.data);
	log_msg("    Specular: %6.2f, %6.2f, %6.2f, %6.2f\n", specular.r, specular.g, specular.b, specular.a);
	log_msg("    SpecularTexture: %.*s\n", specular_texture.len, specular_texture.data);
	log_msg("    Roughness: %6.2f\n", roughness);
	log_msg("    Metallic: %6.2f\n", metallic);

	return 0;
}

static int read_mdl(data_t *data)
{
	const unsigned int obj_cnt = read_int(data);

	log_msg("Objects[%d] = [\n", obj_cnt);

	for (unsigned int i = 0; i < obj_cnt; i++) {
		if (i > 0) {
			log_msg("},\n");
		}
		log_msg("[%d] = {\n", i);
		if (read_object(data)) {
			log_err("Failed to read object\n");
			return 1;
		}
	}

	log_msg("}\n");
	log_msg("]\n");

	const unsigned int mat_cnt = read_int(data);

	log_msg("Materials[%d] = [\n", mat_cnt);

	for (unsigned int i = 0; i < mat_cnt; i++) {
		if (i > 0) {
			log_msg("},\n");
		}
		log_msg("[%d] = {\n", i);
		if (read_material(data)) {
			log_err("Failed to read material\n");
			return 1;
		}
	}

	log_msg("}\n");
	log_msg("]\n");

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		log_err("Usage: %s <path>\n", argv[0]);
		return 1;
	}

	m_stats_t m_stats = { 0 };
	m_init(&m_stats);

	data_t data = { 0 };

	if (read_data(argv[1], &data)) {
		log_err("Failed to read data\n");
		return 1;
	}

	log_msg("File size: %zu KB (%zu B)\n\n", data.size / 1024, data.size);

	if (read_mdl(&data)) {
		log_err("Failed to read mdl\n");
		return 1;
	}

	log_msg("\n");

	m_free(data.data, data.size);

	m_print(stdout);

	return 0;
}
