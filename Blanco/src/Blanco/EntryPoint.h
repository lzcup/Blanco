//this is Entry Point head file
#pragma once

#ifdef BLANCO_PLATFORM_WINDOWS
int main() {
	Blanco::Log::Init();
	BL_CORE_CRITICAL("DANGER!!!");
	BL_INFO("Nice!!!");
	int a = 10;
	BL_WARN("nihao,{0}ge", a);

	auto app = Blanco::CreatApplication();
	app->Run();
	delete app;
}
#endif
