#include "Window.h"

namespace VoxelEngine {
	static bool s_GLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const std::string& title, uint32_t width, uint32_t height, bool vSync)
	{
		m_Width = width;
		m_Height = height;
		m_VSync = vSync;

		if (!s_GLFWinitialized)
		{
			if (!glfwInit())
			{
				ENGINE_CORE_CRITICAL("Failed to initialize GLFW!");
				// TODO: Assert
			}
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWinitialized = true;
		}

		m_WindowHandle = glfwCreateWindow(m_Width, m_Height, title.c_str(), NULL, NULL);

		if (!m_WindowHandle)
		{
			glfwTerminate();
			ENGINE_CORE_CRITICAL("Failed to create GLFW window!");
			// TODO: Assert
		}

		glfwMakeContextCurrent(m_WindowHandle);

		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			});

		if (vSync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwTerminate();
			ENGINE_CORE_CRITICAL("Failed to initialize Glad!");
			// TODO: Assert
		}

		glViewport(0, 0, width, height);

		glEnable(GL_MULTISAMPLE);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	const glm::vec2& Window::GetSize() const
	{
		int width, height;
		glfwGetWindowSize(m_WindowHandle, &width, &height);

		return glm::ivec2(width, height);
	}

	void Window::Update()
	{
		glfwSwapBuffers(m_WindowHandle);
		glfwPollEvents();
	}
}