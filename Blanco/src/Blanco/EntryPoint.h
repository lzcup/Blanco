//this is Entry Point head file

#pragma once
#include "Application.h"

#ifdef BLANCO_PLATFORM_WINDOWS
int main() {
	auto app = Blanco::CreatApplication();
	app->Run();
	delete app;
}
#endif
