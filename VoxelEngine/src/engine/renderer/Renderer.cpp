#include <iostream>
#include <vector>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "engine/core/Log.h"

namespace VoxelEngine {
    Renderer::Renderer()
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

        screenShader = CreateRef<Shader>("assets/shaders/Screen.glsl");
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Renderer::Render(const glm::mat4& camera)
    {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader->Bind();
        screenShader->SetMat4("u_CameraMatrix", camera);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}