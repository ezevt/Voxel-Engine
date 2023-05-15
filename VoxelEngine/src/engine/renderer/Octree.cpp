#include "Octree.h"

#include "engine/core/Log.h"

namespace VoxelEngine {
	const glm::vec3 PPP = glm::vec3(1, 1, 1);
	const glm::vec3 PNP = glm::vec3(1, -1, 1);
	const glm::vec3 PNN = glm::vec3(1, -1, -1);
	const glm::vec3 NPN = glm::vec3(-1, 1, -1);
	const glm::vec3 NNN = glm::vec3(-1, -1, -1);
	const glm::vec3 NNP = glm::vec3(-1, -1, 1);
	const glm::vec3 NPP = glm::vec3(-1, 1, 1);
	const glm::vec3 PPN = glm::vec3(1, 1, -1);
	const glm::vec3 POS[8] = { PNN, PNP, PPN, PPP, NNN, NNP, NPN, NPP };

	Octree::Octree()
		: m_Nodes(*CreateRef<std::vector<uint32_t>>())
	{
		m_StorageBlock = CreateRef<ShaderStorageBuffer>(0);
	}

	Octree::Octree(std::vector<uint32_t>& nodes)
		: m_Nodes(nodes)
	{
		m_StorageBlock = CreateRef<ShaderStorageBuffer>(0);
		m_StorageBlock->SetData(nodes.data(), nodes.size() * sizeof(uint32_t));

		for (int i = 0; i < m_Nodes.size(); i++)
		{
			GetChildPtr(i);
		}
	}

	void Octree::SampleSphere()
	{
		m_Nodes.clear();

		// TODO: Implement function

	}

	void Octree::BindStorage()
	{
		m_StorageBlock->Bind();
	}

	void Octree::UnbindStorage()
	{
		m_StorageBlock->Unbind();
	}

	int Octree::GetChildPtr(int index)
	{
		uint32_t node = m_Nodes[index];
		uint32_t groupOffset = node >> 16;

		return groupOffset;
	}

	std::array<bool, 8> Octree::GetChildEmptyMask(int index)
	{
		uint32_t node = m_Nodes[index];
		uint32_t childMask = (node & 0x0000FF00u) >> 8u;

		std::array<bool, 8> mask;

		for (int i = 0; i < 8; i++)
		{
			mask[i] = (childMask & (1 << i)) == 0;
		}

		return mask;
	}

	std::array<bool, 8> Octree::GetChildLeafMask(int index)
	{
		uint32_t node = m_Nodes[index];
		uint32_t leafMask = node & 0x000000FFu;
		std::array<bool, 8> mask;

		for (int i = 0; i < 8; i++)
		{
			mask[i] = (leafMask & (1 << i)) == 0;
		}

		return mask;
	}

	int Octree::PushNode(uint32_t ptr, std::array<bool, 8> emptyMask, std::array<bool, 8> leafMask)
	{
		// TODO: Implement function

		return 0;
	}
}