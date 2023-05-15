#include "engine/Engine.h"

#include "engine/renderer/ShaderStorageBuffer.h"

using namespace VoxelEngine;

int main()
{
	Log::init();

	Ref<Window> window = CreateRef<Window>("SVO Raytracer", 800, 600, true);
	Ref<Renderer> renderer = CreateRef<Renderer>();
	Ref<Input> input = CreateRef<Input>(window);
	Ref<DebugCamera> camera = CreateRef<DebugCamera>(input);
	camera->Position = glm::vec3(0.0f, 0.0f, 10.0f);

	std::vector<uint32_t> data = { 66046u, 196352u, 719365u, 1111306u, 1504267u, 1798287u, 2010960u, 2383776u, 2760672u, 3081212u, 3244159u, 3317855u, 3448927u, 3604224u, 4127237u, 4521728u, 4997311u, 5075087u, 5308160u, 5821483u, 6094592u, 6583690u, 6924885u, 7205389u, 7533324u, 7879365u, 8190469u, 8581900u, 8917486u, 9045243u, 9153109u, 9436928u, 9961216u, 10485504u, 10945277u, 11012853u, 11206400u, 11685298u, 11992832u, 12474280u, 12782835u, 12914672u, 13238016u, 13729664u, 14169036u, 14418430u, 14484221u, 14565312u, 14946798u, 33023u, 57599u, 12543u, 52479u, 41727u, 51455u, 52479u, 8447u, 65535u, 52479u, 52479u, 65535u, 65535u, 57599u, 61695u, 35071u, 65535u, 43775u, 65535u, 61695u, 65535u, 62207u, 65535u, 65535u, 65535u, 65535u, 65535u, 49407u, 61695u, 20735u, 12543u, 52479u, 50431u, 65535u, 65535u, 52479u, 52479u, 65535u, 65535u, 20735u, 50431u, 65535u, 54783u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 62975u, 28927u, 65535u, 12543u, 65535u, 33023u, 61695u, 56831u, 14335u, 12543u, 60159u, 65535u, 47871u, 65535u, 61695u, 61695u, 65535u, 65535u, 29695u, 12543u, 65535u, 511u, 8447u, 47103u, 49407u, 63743u, 65535u, 19711u, 8959u, 1535u, 61695u, 61695u, 65535u, 65535u, 13311u, 12799u, 4351u, 30719u, 5631u, 35071u, 35071u, 35071u, 767u, 52479u, 52479u, 65535u, 65535u, 52479u, 52479u, 8191u, 49151u, 35583u, 65535u, 44031u, 65535u, 2303u, 36863u, 2815u, 45055u, 65535u, 65535u, 65535u, 65535u, 53247u, 61439u, 65535u, 65535u, 3327u, 2815u, 767u, 52479u, 52479u, 65535u, 65535u, 52479u, 52479u, 65535u, 65535u, 52479u, 65535u, 56831u, 19967u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 65535u, 13311u, 65535u, 24575u, 24575u, 3327u, 1279u, 4095u, 3583u, 4095u, 511u, 61183u, 65535u, 65535u, 65535u, 61183u, 57343u, 49151u, 32767u, 65535u, 65535u, 22527u, 1023u, 4095u, 4095u, 511u, 13311u, 13311u, 767u, 1023u, 4607u, 1279u, 65535u, 65535u, 13311u, 13311u, 16383u, 8191u, 30719u, 511u };

	Ref<Octree> octree = CreateRef<Octree>(data);
	
	float lastFrameTime = window->GetTime();

	while (!window->ShouldClose())
	{
		if (input->IsKeyPressed(Key::Escape)) break;

		float currentTime = window->GetTime();
		float timeStep = (currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		auto windowSize = window->GetSize();
		camera->Update(timeStep, windowSize.x, windowSize.y);

		renderer->Render(camera->GetViewMatrix(), camera->GetProjectionMatrix(), windowSize, octree);

		window->Update();
	}

	return 0;
}