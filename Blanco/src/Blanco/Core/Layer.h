#pragma once
#include "Core.h"
#include "Blanco/Events/Event.h"
#include "Blanco/Core/TimeStep.h"

namespace Blanco 
{
	class Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(TimeStep ts) {};
		virtual void OnEvent(Event& e) {};
		virtual void OnImguiRender() {};

		inline const char* GetName() { return m_LayerName; }
	protected:
		const char* m_LayerName;
	};
}
