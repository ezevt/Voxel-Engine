#include "DebugCamera.h"

#include "engine/core/Log.h"

namespace VoxelEngine {
	DebugCamera::DebugCamera(Input* input)
		: m_Input(input)
	{
	}

	void DebugCamera::Update(float ts, uint32_t width, uint32_t height)
	{
		if (m_Input->IsKeyPressed(Key::W))
		{
			Position += m_Speed * m_Orientation * ts;
		}
		if (m_Input->IsKeyPressed(Key::A))
		{
			Position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up)) * ts;
		}
		if (m_Input->IsKeyPressed(Key::S))
		{
			Position += m_Speed * -m_Orientation * ts;
		}
		if (m_Input->IsKeyPressed(Key::D))
		{
			Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up)) * ts;
		}
		if (m_Input->IsKeyPressed(Key::Space))
		{
			Position += m_Speed * m_Up * ts;
		}
		if (m_Input->IsKeyPressed(Key::LeftControl))
		{
			Position += m_Speed * -m_Up * ts;
		}
		if (m_Input->IsKeyPressed(Key::LeftShift))
		{
			m_Speed = Speed * 7.0f;
		}
		else
		{
			m_Speed = Speed;
		}

		if (m_Input->IsMouseButtonPressed(Mouse::ButtonLeft))
		{

			m_Input->HideCursor(true);

			if (m_FirstClick)
			{
				m_Input->SetMousePosition((width / 2), (height / 2));
				m_FirstClick = false;
			}


			glm::vec2 mousePos = m_Input->GetMousePosition();

			float rotX = Sensitivity * (float)(mousePos.y - (height / 2)) / height;
			float rotY = Sensitivity * (float)(mousePos.x - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

			if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(90.0f))
			{
				m_Orientation = newOrientation;
			}

			m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

			m_Input->SetMousePosition((width / 2), (height / 2));
		}
		else
		{
			m_FirstClick = true;
			m_Input->HideCursor(false);
		}

		float FOVdeg = 100.0f;
		float nearPlane = 0.01f;
		float farPlane = 5000.0f;

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = glm::lookAt(Position, Position + m_Orientation, m_Up);
		if (width != 0 && height != 0)
			projection = glm::perspective(glm::radians(FOVdeg), (float)((float)width / (float)height), nearPlane, farPlane);

		m_Projection = projection;
		m_View = view;
	}
}