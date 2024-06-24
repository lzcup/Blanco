//this is Entry Point head file
#pragma once

#ifdef BLANCO_PLATFORM_WINDOWS
int main() {
	BL_PROFILE_BEGIN_SESSION("start","Profile.StartUp.json");
	Blanco::Log::Init();
	auto app = Blanco::CreatApplication();
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("runtime", "Profile.Runtime.json");
	app->Run();
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("destory", "Profile.ShutDown.json");
	delete app;
	BL_PROFILE_END_SESSION();
}
#endif
