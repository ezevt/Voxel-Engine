#include "Renderer.h"

#include <iostream>
#include <vector>
#include <random>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace VoxelEngine {
    static float GenerateRandomFloat(float min, float max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);
        float randomFloat = dis(gen);
        return randomFloat;
    }

    Renderer::Renderer(Ref<Window> window)
        : m_Window(window)
    {
        ENGINE_CORE_INFO("Initializing renderer");

        float vertices[] = {
            // Position        // Color
            -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGetError();
        // Create the vertex array object (VAO)
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glm::vec2 windowSize = m_Window->GetSize();

        FramebufferSpecification fbSpecMS;
        fbSpecMS.Width = windowSize.x;
        fbSpecMS.Height = windowSize.y;
        fbSpecMS.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
        fbSpecMS.Samples = 4;

        m_FramebufferMS = CreateRef<Framebuffer>(fbSpecMS);

        FramebufferSpecification fbSpec;
        fbSpec.Width = windowSize.x;
        fbSpec.Height = windowSize.y;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

        m_Framebuffer = CreateRef<Framebuffer>(fbSpec);

        m_RaytraceShader = CreateRef<Shader>("assets/shaders/RayTrace.glsl");
        m_PostProcessShader = CreateRef<Shader>("assets/shaders/PostProcess.glsl");
        m_QuadShader = CreateRef<Shader>("assets/shaders/Quad.glsl");

        m_SettingsUniformBuffer = CreateRef<UniformBuffer>(sizeof(DefaultSettings));

        m_JitterSettingsBuffer.CurrentJitter = glm::vec2((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        m_JitterSettingsBuffer.PreviousJitter = glm::vec2((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        m_JitterSettingsBuffer.JitterScale = 0.0f;
        m_JitterSettingsUniformBuffer = CreateRef<UniformBuffer>(sizeof(JitterSettings));
        m_JitterSettingsUniformBuffer->SetData(&m_JitterSettingsBuffer, sizeof(JitterSettings));
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Renderer::Render(const glm::mat4& cameraView, const glm::mat4& cameraProjection, Ref<Octree> octree)
    {

        glm::vec2 screenSize = m_Window->GetSize();
        float time = m_Window->GetTime();

        m_SettingsBuffer.CameraView = cameraView;
        m_SettingsBuffer.CameraProjection = cameraProjection;
        m_SettingsBuffer.ScreenSize = screenSize;
        m_SettingsBuffer.Time = time;

        m_SettingsUniformBuffer->SetData(&m_SettingsBuffer, sizeof(DefaultSettings));
        m_SettingsUniformBuffer->Bind(0);

        m_JitterSettingsUniformBuffer->SetData(&m_JitterSettingsBuffer, sizeof(JitterSettings));
        m_JitterSettingsUniformBuffer->Bind(1);

        glm::vec2 fbSize = m_Framebuffer->GetSize();
        if (screenSize != fbSize)
        {
            m_Framebuffer->Resize(screenSize.x, screenSize.y);
            m_FramebufferMS->Resize(screenSize.x, screenSize.y);
        }

        {
            m_FramebufferMS->Bind();
            glClearColor(0.1, 0.3, 0.2, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            m_RaytraceShader->Bind();

            octree->BindStorage(3);

            m_RaytraceShader->SetMat4("u_CameraView", cameraView);
            m_RaytraceShader->SetMat4("u_CameraProjection", cameraProjection);
            m_RaytraceShader->SetFloat2("u_ScreenSize", screenSize);
            m_RaytraceShader->SetFloat("u_Time", time);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            octree->UnbindStorage();

            m_RaytraceShader->Unbind();

            m_FramebufferMS->Unbind();
        }

        {
            m_Framebuffer->Bind();

            m_PostProcessShader->Bind();

            m_FramebufferMS->BindAttachmentTexture(0, 1);
            m_Framebuffer->BindAttachmentTexture(0, 0);

            m_PostProcessShader->SetInt("u_ColorTextureMS", 1);
            m_PostProcessShader->SetInt("u_ColorTexture", 0);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            m_PostProcessShader->Unbind();
            m_Framebuffer->Unbind();
        }

        {
            glClearColor(0.1, 0.3, 0.2, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            m_QuadShader->Bind();

            m_Framebuffer->BindAttachmentTexture(0, 0);

            m_QuadShader->SetInt("u_ColorTexture", 0);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            m_QuadShader->Unbind();
        }

        m_JitterSettingsBuffer.PreviousJitter = m_JitterSettingsBuffer.CurrentJitter;
        m_JitterSettingsBuffer.CurrentJitter = glm::vec2(GenerateRandomFloat(0.0f, 1.0f), GenerateRandomFloat(0.0f, 1.0f));
    }
}