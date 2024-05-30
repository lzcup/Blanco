#include <Blanco.h>

class ExampleLayer :public Blanco::Layer {
public:
	ExampleLayer():Layer("Example") {};
	~ExampleLayer() {};

	void OnUpdate() override
	{
		if (Blanco::Input::IsKeyPressed(BL_KEY_A))
			BL_TRACE("A is pressed(poll)");
	}

	void OnEvent(Blanco::Event& event) override
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
		PushOverLayer(new Blanco::ImguiLayer());
	};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp();
}
