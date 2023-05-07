#include "engine/Engine.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Window window("SVO Raytracer", 800, 600, true);
	Renderer renderer;

	while (!window.ShouldClose())
	{
		renderer.Render();

		window.Update();
	}

	return 0;
}