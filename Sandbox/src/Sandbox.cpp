#include "engine/Engine.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Window window("SVO Raytracer", 800, 600, true);
	Renderer renderer;
	Input input(&window);
	DebugCamera camera(&input);
	camera.Position = glm::vec3(0.0f, 0.0f, 10.0f);

	float lastFrameTime = window.GetTime();

	while (!window.ShouldClose())
	{
		if (input.IsKeyPressed(Key::Escape)) break;

		float currentTime = window.GetTime();
		float timeStep = (currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		auto windowSize = window.GetSize();
		camera.Update(timeStep, windowSize.x, windowSize.y);

		renderer.Render(camera.GetViewMatrix(), camera.GetProjectionMatrix(), windowSize);

		window.Update();
	}

	return 0;
}