#include "ShaderStorageBuffer.h"

#include "glad/glad.h"

namespace VoxelEngine {
	ShaderStorageBuffer::ShaderStorageBuffer()
	{
		glCreateBuffers(1, &m_RendererID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_STREAM_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void ShaderStorageBuffer::SetData(void* data, size_t size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void ShaderStorageBuffer::Bind(uint32_t binding)
	{
		m_Binding = binding;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID);
	}

	void ShaderStorageBuffer::Unbind()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, 0);
	}
}