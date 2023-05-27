#pragma once

#include "engine/core/Base.h";
#include "engine/renderer/Shader.h";

namespace VoxelEngine {
	class ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer();
		~ShaderStorageBuffer();

		void SetData(void* data, size_t size);

		void Bind(uint32_t binding);
		void Unbind();

	private:
		uint32_t m_RendererID;
		uint32_t m_Binding;
	};
}

