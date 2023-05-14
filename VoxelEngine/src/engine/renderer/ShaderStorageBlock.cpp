#include "ShaderStorageBlock.h"

#include "glad/glad.h"

namespace VoxelEngine {
	ShaderStorageBlock::ShaderStorageBlock(Ref<Shader>& shader, const std::string& name, uint32_t binding, void* data, size_t size)
		: m_Binding(binding)
	{
		glGenBuffers(1, &m_RendererID);
		uint32_t blockIndex = glGetProgramResourceIndex(shader->m_RendererID, GL_SHADER_STORAGE_BLOCK, name.c_str());
		glShaderStorageBlockBinding(shader->m_RendererID, blockIndex, binding);
		glBindBuffer(GL_SHADER_STORAGE_BLOCK, m_RendererID);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	ShaderStorageBlock::~ShaderStorageBlock()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void ShaderStorageBlock::Bind()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID);
	}

	void ShaderStorageBlock::Unbind()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Binding, 0);
	}
}