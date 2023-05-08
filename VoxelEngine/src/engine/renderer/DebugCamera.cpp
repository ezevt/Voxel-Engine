#include "DebugCamera.h"

#include "engine/core/Log.h"

namespace VoxelEngine {
	DebugCamera::DebugCamera(Input* input)
		: m_Input(input)
	{
	}

	void DebugCamera::Update(float ts, uint32_t width, uint32_t height)
	{
		Resolution = { width, height };

		if (m_Input->IsKeyPressed(Key::W))
		{
			Position += speed * Orientation * ts;
		}
		if (m_Input->IsKeyPressed(Key::A))
		{
			Position += speed * -glm::normalize(glm::cross(Orientation, Up)) * ts;
		}
		if (m_Input->IsKeyPressed(Key::S))
		{
			Position += speed * -Orientation * ts;
		}
		if (m_Input->IsKeyPressed(Key::D))
		{
			Position += speed * glm::normalize(glm::cross(Orientation, Up)) * ts;
		}
		if (m_Input->IsKeyPressed(Key::Space))
		{
			Position += speed * Up * ts;
		}
		if (m_Input->IsKeyPressed(Key::LeftControl))
		{
			Position += speed * -Up * ts;
		}
		if (m_Input->IsKeyPressed(Key::LeftShift))
		{
			speed = 11.1f;
		}
		else
		{
			speed = 1.5f;
		}

		if (m_Input->IsMouseButtonPressed(Mouse::ButtonLeft))
		{

			// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				m_Input->SetMousePosition((width / 2), (height / 2));
				firstClick = false;
			}


			glm::vec2 mousePos = m_Input->GetMousePosition();

			float rotX = sensitivity * (float)(mousePos.y - (height / 2)) / height;
			float rotY = sensitivity * (float)(mousePos.x - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(90.0f))
			{
				Orientation = newOrientation;
			}

			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			m_Input->SetMousePosition((width / 2), (height / 2));
		}
		else
		{
			firstClick = true;
		}

		float FOVdeg = 90.0f;
		float nearPlane = 0.01f;
		float farPlane = 5000.0f;

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(Position, Position + Orientation, Up);
		if (width != 0 && height != 0)
			projection = glm::perspective(glm::radians(FOVdeg), (float)((float)width / (float)height), nearPlane, farPlane);

		Projection = projection;
		View = view;
		Matrix = projection * view;
	}
}