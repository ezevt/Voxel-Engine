#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "engine/core/Base.h"
#include "DebugCamera.h"

namespace VoxelEngine {
    struct OctreeNode {
        uint32_t ChildPtr;
        uint8_t ChildIsEmpty;
        uint8_t ChildIsLeaf;
        uint32_t color;
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void Render(const glm::mat4& camera);

    private:
        Ref<Shader> screenShader;

        uint32_t m_VBO;
        uint32_t m_VAO;
        uint32_t m_EBO;
    };
}