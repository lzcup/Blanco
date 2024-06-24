#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include "GameLayer.h"


class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { 
		PushLayer(new GameLayer());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
