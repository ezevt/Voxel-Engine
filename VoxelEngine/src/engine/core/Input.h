#pragma once

#include "Window.h"
#include "MouseCodes.h"
#include "KeyCodes.h"

#include "glm/glm.hpp"

namespace VoxelEngine {
	class Input
	{
	public:
		Input(Ref<Window>& window);
		~Input();

		bool IsKeyPressed(KeyCode key);
		
		bool IsMouseButtonPressed(MouseCode button);
		glm::vec2 GetMousePosition();

		void SetMousePosition(uint32_t x, uint32_t y);

		void HideCursor(bool state);
	private:
		Ref<Window>& m_Window;
	};
}