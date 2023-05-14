#pragma once

#include "engine/core/Base.h"
#include "ShaderStorageBlock.h"

#include <vector>

namespace VoxelEngine {
	class Octree
	{
	public:
		Octree(std::vector<uint32_t>& nodes, Ref<Shader>& shader);

		void BindStorage() { m_StorageBlock->Bind(); }
		void UnbindStorage() { m_StorageBlock->Unbind(); }

	private:
		Ref<ShaderStorageBlock> m_StorageBlock;
	};
}
