#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Blanco {
	class BlancoPelota :public Application {
	public:
		BlancoPelota() :Application("BlancoPelota") {
			PushLayer(new EditorLayer());
		};
		~BlancoPelota() override {};
	};

	Application* CreatApplication() {
		return new BlancoPelota();
	}
}
