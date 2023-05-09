#pragma once

#include "engine/core/Input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace VoxelEngine {
	class DebugCamera
	{
	public:
		DebugCamera(Input* input);

		void Update(float ts, uint32_t width, uint32_t height);

		glm::mat4 GetCameraMatrix() { return m_Projection * m_View; }

		glm::vec3 Position = glm::vec3(0);
		float Speed = 1.5f;
		float Sensitivity = 100.0f;
	private:
		bool m_FirstClick = true;
		float m_Speed;

		Input* m_Input;

		glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
	};
}

