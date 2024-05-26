#pragma once
#include "Core.h"
#include "Events/Event.h"

namespace Blanco 
{
	class BL_API Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void Detach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};

		inline const char* GetName() { return m_LayerName; }
	protected:
		const char* m_LayerName;
	};
}
