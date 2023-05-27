#pragma once

#include "engine/core/Base.h"

namespace VoxelEngine {
	class UniformBuffer
	{
	public:
		UniformBuffer(uint32_t size);
		~UniformBuffer();
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		void Bind(uint32_t binding);
		void Unbind();
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Binding = 0;
	};
}