//this is Entry Point head file
#pragma once

#ifdef BLANCO_PLATFORM_WINDOWS

int main(int argc, char** argv) {
	Blanco::Log::Init();

	BL_PROFILE_BEGIN_SESSION("start","Profile.StartUp.json");
	auto app = Blanco::CreatApplication({ argc, argv });
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("runtime", "Profile.Runtime.json");
	app->Run();
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("destory", "Profile.ShutDown.json");
	delete app;
	BL_PROFILE_END_SESSION();
}
#endif
