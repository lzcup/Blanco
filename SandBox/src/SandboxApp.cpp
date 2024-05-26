#include <Blanco.h>

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer():Layer("Example") {};
	~ExampleLayer() {};

	void OnUpdate() override
	{
		BL_INFO("ExampleLayer Updated");
	}

	void OnEvent(Blanco::Event& event) override
	{
		BL_INFO("Event:{0}(ExampleLayer)",event.ToString());
	}
};


class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { PushLayer(new ExampleLayer()); };
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
