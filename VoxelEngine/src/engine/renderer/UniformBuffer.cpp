#include "UniformBuffer.h"

#include "glad/glad.h"

namespace VoxelEngine {
	UniformBuffer::UniformBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);

		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::Bind(uint32_t binding)
	{
		m_Binding = binding;
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_RendererID);
	}
	 
	void UniformBuffer::Unbind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, 0);
	}
}
