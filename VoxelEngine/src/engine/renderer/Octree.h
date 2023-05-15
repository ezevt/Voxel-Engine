#pragma once

#include "engine/core/Base.h"
#include "ShaderStorageBuffer.h"

#include <vector>

namespace VoxelEngine {
	class Octree
	{
	public:
		Octree();
		Octree(std::vector<uint32_t>& nodes);

		void SampleSphere();

		void BindStorage();
		void UnbindStorage();

	private:
		int GetChildPtr(int index);
		std::array<bool, 8> GetChildEmptyMask(int index);
		std::array<bool, 8> GetChildLeafMask(int index);

		int PushNode(uint32_t ptr, std::array<bool, 8> emptyMask, std::array<bool, 8> leafMask);

	private:
		Ref<ShaderStorageBuffer> m_StorageBlock;
		std::vector<uint32_t>& m_Nodes;
	};
}
