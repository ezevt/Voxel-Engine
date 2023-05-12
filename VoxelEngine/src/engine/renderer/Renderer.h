#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "engine/core/Base.h"
#include "DebugCamera.h"

namespace VoxelEngine {
    struct OctreeNode {
        uint32_t data;
        // if leaf, data stores color
        // if parent, data stores 16 bit child index, 8 bit child empty mask, 8 bit child leaf mask
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void Render(const glm::mat4& cameraView, const glm::mat4& cameraProjection, const glm::vec2& screenSize);

    private:
        Ref<Shader> screenShader;

        uint32_t m_VBO;
        uint32_t m_VAO;
        uint32_t m_EBO;
    };
}