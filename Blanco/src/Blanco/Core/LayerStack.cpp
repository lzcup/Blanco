#include "BLpch.h"
#include "LayerStack.h"

namespace Blanco
{
	LayerStack::LayerStack():m_LayerInsertIndex(0)
	{
	}
	LayerStack::~LayerStack()
	{
		BL_PROFILE_FUNCTION();

		for (Layer* layer : m_LayerStack)
			delete layer;
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}
	void LayerStack::PushOverLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		m_LayerStack.emplace_back(layer);
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != m_LayerStack.end()) {
			m_LayerStack.erase(it);
			m_LayerInsertIndex--;
		}

	}
	void LayerStack::PopOverLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != m_LayerStack.end())
			m_LayerStack.erase(it);
	}
}