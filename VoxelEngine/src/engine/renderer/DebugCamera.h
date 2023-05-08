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
		// Stores the main vectors of the camera
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 Matrix = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::vec2 Resolution = glm::vec2(1.0f);
		// Prevents the camera from jumping around when first clicking left click
		bool firstClick = true;

		Input* m_Input;

		float speed = 0.1f;
		float sensitivity = 100.0f;

		DebugCamera(Input* input);

		void Update(float ts, uint32_t width, uint32_t height);
	};
}

