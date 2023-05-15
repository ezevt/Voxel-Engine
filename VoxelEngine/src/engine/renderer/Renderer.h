#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "engine/core/Base.h"
#include "DebugCamera.h"
#include "Octree.h"

namespace VoxelEngine {
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void Render(const glm::mat4& cameraView, const glm::mat4& cameraProjection, const glm::vec2& screenSize, Ref<Octree> octree = nullptr);

        const Ref<Shader>& GetShader() const { return m_ScreenShader; }

    private:
        Ref<Shader> m_ScreenShader;

        uint32_t m_VBO;
        uint32_t m_VAO;
        uint32_t m_EBO;
    };
}