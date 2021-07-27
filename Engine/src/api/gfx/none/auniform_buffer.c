#include "pch.h"
#ifdef GAPI_NONE
#include "api/gfx/auniform_buffer.h"

struct AUniformBuffer {
	uint id;
};

AUniformBuffer* auniformbuffer_create() {
	AUniformBuffer* uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer* uniform_buffer) {
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_init_static(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size) {

}

void auniformbuffer_init_dynamic(AUniformBuffer* uniform_buffer, uint uniforms_size) {

}

void auniformbuffer_bind_base(AUniformBuffer* uniform_buffer, uint index) {

}

void auniformbuffer_set_data(AUniformBuffer* uniform_buffer, const void* uniforms, uint uniforms_size) {

}
#endif