#include <Blanco.h>

class SandboxApp :public Blanco::Application {
public:
	SandboxApp() {};
	~SandboxApp() override{};
};

Blanco::Application* Blanco::CreatApplication() {
	return new SandboxApp;
}
