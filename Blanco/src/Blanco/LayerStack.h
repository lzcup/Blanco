#pragma once
#include "Core.h"
#include "Layer.h"

namespace Blanco 
{
    class BL_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverLayer(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverLayer(Layer* layer);

        inline std::vector<Layer*>::iterator begin() { return m_LayerStack.begin(); }
        inline std::vector<Layer*>::iterator end() { return m_LayerStack.end(); }
    private:
        std::vector<Layer*> m_LayerStack;
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}