#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Core/TimeStep.h"

namespace Blanco 
{
	class BL_API Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(TimeStep ts) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnImguiRender() {};

		inline const char* GetName() { return m_LayerName; }
	protected:
		const char* m_LayerName;
	};
}
