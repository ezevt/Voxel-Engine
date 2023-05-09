#include "engine/Engine.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Window window("SVO Raytracer", 800, 600, true);
	Renderer renderer;
	Input input(&window);
	DebugCamera camera(&input);

	float lastFrameTime = window.GetTime();

	while (!window.ShouldClose())
	{
		if (input.IsKeyPressed(Key::Escape)) break;

		float currentTime = window.GetTime();
		float timeStep = (currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		auto windowSize = window.GetSize();
		camera.Update(timeStep, windowSize.x, windowSize.y);

		renderer.Render(camera.GetCameraMatrix());

		window.Update();
	}

	return 0;
}