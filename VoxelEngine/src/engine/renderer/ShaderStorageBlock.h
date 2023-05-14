#pragma once

#include "engine/core/Base.h"
#include "engine/renderer/Shader.h";

namespace VoxelEngine {
	class ShaderStorageBlock
	{
	public:
		ShaderStorageBlock(Ref<Shader>& shader, const std::string& name, uint32_t binding, void* data, size_t size);
		~ShaderStorageBlock();

		void Bind();
		void Unbind();

	private:
		uint32_t m_RendererID;
		uint32_t m_Binding;
	};
}

