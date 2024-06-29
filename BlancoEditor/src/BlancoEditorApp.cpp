#include <Blanco.h>
#include <Blanco/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Blanco {
	class BlancoEditorApp :public Application {
	public:
		BlancoEditorApp() :Application("BlancoEditor") {
			PushLayer(new EditorLayer());
		};
		~BlancoEditorApp() override {};
	};

	Application* CreatApplication() {
		return new BlancoEditorApp();
	}
}
