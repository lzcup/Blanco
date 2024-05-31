#include <Blanco.h>
#include "imgui.h"

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer():Layer("Example") {};
	~ExampleLayer() {};

	virtual void OnUpdate() override
	{
		if (Blanco::Input::IsKeyPressed(BL_KEY_A))
			BL_TRACE("A is pressed(poll)");
	}

	virtual void OnImguiRender() override {
		ImGui::Begin("hello");;
		ImGui::Text("Baiwan");
		ImGui::End();
	};

	virtual void OnEvent(Blanco::Event& event) override
	{
		if (event.GetEventType() == Blanco::EventType::KeyPressed) {
			Blanco::KeyPressedEvent& e = dynamic_cast<Blanco::KeyPressedEvent&>(event);
			char key = (char)e.GetKeyCode();
			BL_TRACE("{0} is pressed(event)", key);
		}
	}
};


class SandboxApp :public Blanco::Application {
public:
	SandboxApp() { 
		PushLayer(new ExampleLayer());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
