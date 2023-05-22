#include "Octree.h"

#include "engine/core/Log.h"
#include <bitset>

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
		: m_Nodes(std::vector<uint32_t>())
	{
		m_StorageBlock = CreateRef<ShaderStorageBuffer>(0);
	}

	Octree::Octree(std::vector<uint32_t>& nodes)
		: m_Nodes(nodes)
	{
		m_StorageBlock = CreateRef<ShaderStorageBuffer>(0);
		m_StorageBlock->SetData(m_Nodes.data(), m_Nodes.size() * sizeof(uint32_t));

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

	void Octree::SampleRandom(uint32_t lod)
	{
		m_Nodes.clear();

		// TODO: fix crash due to stack overflow

		std::array<bool, 8> emptyMask = { false, false, false, false, false, false, false, false };
		std::array<bool, 8> leafMask = { false, false, false, false, false, false, false, false };

		for (int i = 0; i < 8; i++)
		{
			int state = (int)round((float)rand() / RAND_MAX * 3.0f);
		
			if (state == 0)
			{
				emptyMask[i] = false;
				leafMask[i] = false;
			}
			else if (state == 1)
			{
				if (1 != lod) continue;

				emptyMask[i] = true;
				leafMask[i] = true;
			}
			else
			{
				if (lod == 1) continue;
		
				emptyMask[i] = true;
				leafMask[i] = false;
			}
		}

		PushNode(0, emptyMask, leafMask);
		PushRandomBlock(0, lod, 2);

		//for (int i = 0; i < m_Nodes.size(); i++)
		//{
		//	std::string str = std::bitset<32>(m_Nodes[i]).to_string();
		//	str.insert(0, ": ");
		//	str.insert(0, std::to_string(i));
		//	ENGINE_TRACE(str);
		//}

		m_StorageBlock->SetData(m_Nodes.data(), m_Nodes.size() * sizeof(uint32_t));
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
			mask[i] = (childMask & (1 << (7 - i))) != 0;
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
			mask[i] = (leafMask & (1 << (7 - i))) != 0;
		}

		return mask;
	}

	void Octree::SetChildPtr(int index, uint16_t ptr)
	{
		uint32_t node = m_Nodes[index];
		node = node & 0x0000FFFF;
		node |= (ptr << 16);
		m_Nodes[index] = node;
	}

	void Octree::PushRandomBlock(int parentIndex, uint32_t maxLod, uint32_t lod)
	{
		std::array<bool, 8> parentEmptyMask = GetChildEmptyMask(parentIndex);
		std::array<bool, 8> parentLeafMask = GetChildLeafMask(parentIndex);

		uint16_t blockIndex = m_Nodes.size();

		for (int i = 0; i < 8; i++)
		{
			if (parentEmptyMask[i] == true)
			{
				if (parentLeafMask[i] == false)
				{
					std::array<bool, 8> emptyMask = { false, false, false, false, false, false, false, false };
					std::array<bool, 8> leafMask = { false, false, false, false, false, false, false, false };

					for (int i = 0; i < 8; i++)
					{
						int state = (int)round((float)rand() / RAND_MAX * 3.0f);

						if (state == 0)
						{
							emptyMask[i] = false;
							leafMask[i] = false;
						}
						else if (state == 1)
						{
							if (lod != maxLod)
							{
								emptyMask[i] = true;
								leafMask[i] = false;
								continue;
							}
							emptyMask[i] = true;
							leafMask[i] = true;
						}
						else
						{
							if (lod == maxLod)
							{
								emptyMask[i] = true;
								leafMask[i] = true;
								continue;
							}

							emptyMask[i] = true;
							leafMask[i] = false;
						}
					}

					PushNode(0, emptyMask, leafMask);
				}
				else
				{
					uint32_t color = ((uint32_t)rand()) & 0x00ffffff;

					PushNode(color | 0xff000000);
				}
			}
		}

		SetChildPtr(parentIndex, blockIndex);

		int accumulatedOffset = 0;
		for (int i = 0; i < 8; i++)
		{
			if (parentEmptyMask[i] == true)
			{
				if (parentLeafMask[i] == false)
				{
					PushRandomBlock(blockIndex + accumulatedOffset, maxLod, lod+1);
				}
				accumulatedOffset++;
			}
		}
	}

	int Octree::PushNode(uint16_t ptr, std::array<bool, 8>& emptyMask, std::array<bool, 8>& leafMask)
	{
		uint32_t node = (ptr << 16);

		for (int i = 0; i < 8; i++)
		{
			node |= (emptyMask[i] << (15 - i));
			node |= (leafMask[i] << (7 - i));
		}

		m_Nodes.push_back(node);
		return (int)m_Nodes.size() - 1;
	}

	int Octree::PushNode(uint32_t data)
	{
		m_Nodes.push_back(data);
		return m_Nodes.size() - 1;
	}
}