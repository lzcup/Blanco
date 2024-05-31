#include "BLpch.h"
#include "LayerStack.h"

namespace Blanco
{
	LayerStack::LayerStack():m_LayerInsertIndex(0)
	{
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_LayerStack)
			delete layer;
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerStack.emplace(m_LayerStack.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}
	void LayerStack::PushOverLayer(Layer* layer)
	{
		m_LayerStack.emplace_back(layer);
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != m_LayerStack.end()) {
			m_LayerStack.erase(it);
			m_LayerInsertIndex--;
		}

	}
	void LayerStack::PopOverLayer(Layer* layer)
	{
		auto it = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (it != m_LayerStack.end())
			m_LayerStack.erase(it);
	}
}