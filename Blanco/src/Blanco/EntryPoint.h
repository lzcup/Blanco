//this is Entry Point head file
#pragma once

#ifdef BLANCO_PLATFORM_WINDOWS
int main() {
	Blanco::Log::Init();

	auto app = Blanco::CreatApplication();
	app->Run();
	delete app;
}
#endif
