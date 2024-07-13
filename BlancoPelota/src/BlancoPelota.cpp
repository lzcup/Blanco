#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Blanco {
	class BlancoPelota :public Application {
	public:
		BlancoPelota(ApplicationCommandLineArgs args) :Application("BlancoPelota",args) {
			PushLayer(new EditorLayer());
		};
		~BlancoPelota() override {};
	};

	Application* CreatApplication(ApplicationCommandLineArgs args) {
		return new BlancoPelota(args);
	}
}
