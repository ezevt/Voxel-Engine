#include "engine/Engine.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Window window("SVO Raytracer", 800, 600, true);

	while (!window.ShouldClose())
	{
		window.Update();
	}

	return 0;
}