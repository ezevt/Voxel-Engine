#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "UniformBuffer.h"
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

        void Render(const glm::mat4& cameraView, const glm::mat4& cameraProjection, Ref<Octree> octree);

        const Ref<Shader>& GetShader() const { return m_RaytraceShader; }

    private:
        Ref<Shader> m_RaytraceShader;
        Ref<Shader> m_PostProcessShader;
        Ref<Shader> m_QuadShader;
        Ref<Framebuffer> m_Framebuffer;
        Ref<Framebuffer> m_FramebufferMS;

        Ref<Window> m_Window;

        uint32_t m_VBO;
        uint32_t m_VAO;
        uint32_t m_EBO;

        struct DefaultSettings
        {
            glm::mat4 CameraProjection;
            glm::mat4 CameraView;
            glm::vec2 ScreenSize;
            float Time;
        };
        DefaultSettings m_SettingsBuffer;
        Ref<UniformBuffer> m_SettingsUniformBuffer;

        struct JitterSettings
        {
            glm::vec2 PreviousJitter;
            glm::vec2 CurrentJitter;
            float JitterScale;
        };
        JitterSettings m_JitterSettingsBuffer;
        Ref<UniformBuffer> m_JitterSettingsUniformBuffer;
    };
}