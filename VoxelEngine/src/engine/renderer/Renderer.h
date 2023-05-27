#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Framebuffer.h"
#include "engine/core/Base.h"
#include "DebugCamera.h"
#include "Octree.h"

namespace VoxelEngine {
    class Renderer
    {
    public:
        Renderer(Ref<Window> window);
        ~Renderer();

        void Render(const glm::mat4& cameraView, const glm::mat4& cameraProjection, Ref<Octree> octree = nullptr, bool denoise = true);

        const Ref<Shader>& GetShader() const { return m_RaytraceShader; }

    private:
        Ref<Shader> m_RaytraceShader;
        Ref<Shader> m_QuadShader;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Window> m_Window;

        uint32_t m_VBO;
        uint32_t m_VAO;
        uint32_t m_EBO;
    };
}