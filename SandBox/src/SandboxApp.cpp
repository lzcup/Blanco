#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "SandBox2D.h"

class SandboxApp :public Blanco::Application {
public:
	SandboxApp(Blanco::ApplicationCommandLineArgs args) :Application("Sandbox", args) {
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication(ApplicationCommandLineArgs args) {
	return new SandboxApp(args);
}
