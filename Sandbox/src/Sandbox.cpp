#include "engine/Engine.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Window window("SVO Raytracer", 800, 600, true);
	Renderer renderer;
	Input input(&window);
	DebugCamera camera(&input);

	while (!window.ShouldClose())
	{
		if (input.IsKeyPressed(Key::Escape)) break;

		camera.Update(1.0f / 60.0f, 800, 600);

		renderer.Render(camera.GetCameraMatrix());

		window.Update();
	}

	return 0;
}