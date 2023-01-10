#include "api/gfx/gfx_api_uniform_buffer.h"
#include "gfx_none_types.h"

AUniformBuffer *auniformbuffer_create_static(ARenderer *renderer, uint slot, uint data_size, const void *data)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

AUniformBuffer *auniformbuffer_create_dynamic(ARenderer *renderer, uint slot, uint data_size)
{
	AUniformBuffer *uniform_buffer = m_malloc(sizeof(AUniformBuffer));
	return uniform_buffer;
}

void auniformbuffer_delete(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
	m_free(uniform_buffer, sizeof(AUniformBuffer));
}

void auniformbuffer_upload(AUniformBuffer *uniform_buffer, ARenderer *renderer, const void *data, uint data_size)
{
}

void auniformbuffer_bind_vs(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
}

void auniformbuffer_bind_ps(AUniformBuffer *uniform_buffer, ARenderer *renderer)
{
}
