#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "SandBox2D.h"

class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { 
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
