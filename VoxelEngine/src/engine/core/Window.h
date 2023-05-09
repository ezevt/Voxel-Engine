#pragma once

#include "Base.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <string>

namespace VoxelEngine {
	class Window
	{
	public:
		Window(const std::string& title, uint32_t width, uint32_t height, bool vSync = false);
		~Window();

		const glm::vec2& GetSize() const;

		bool ShouldClose() const { return glfwWindowShouldClose(m_WindowHandle); }
		float GetTime() const { return glfwGetTime(); }

		void Update();
	private:
		friend class Input;
	private:
		GLFWwindow* m_WindowHandle;

		uint32_t m_Width, m_Height;
		bool m_VSync;
	};
}