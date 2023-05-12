#include "Input.h"

#include "GLFW/glfw3.h"

namespace VoxelEngine {
	Input::Input(Window* window)
	{
		m_Window = window;
	}

	Input::~Input()
	{
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		return glfwGetKey(m_Window->m_WindowHandle, key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return glfwGetMouseButton(m_Window->m_WindowHandle, button) == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(m_Window->m_WindowHandle, &x, &y);

		return glm::vec2(x, y);
	}

	void Input::SetMousePosition(uint32_t x, uint32_t y)
	{
		glfwSetCursorPos(m_Window->m_WindowHandle, x, y);
	}

	void Input::HideCursor(bool state)
	{
		if (state)
			glfwSetInputMode(m_Window->m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else
			glfwSetInputMode(m_Window->m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}