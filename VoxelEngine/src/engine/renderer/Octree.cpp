#include "Octree.h"

namespace VoxelEngine {
	Octree::Octree(std::vector<uint32_t>& nodes, Ref<Shader>& shader)
	{
		m_StorageBlock = CreateRef<ShaderStorageBlock>(shader, "Octree", 0, nodes.data(), nodes.size()*sizeof(uint32_t));
	}
}