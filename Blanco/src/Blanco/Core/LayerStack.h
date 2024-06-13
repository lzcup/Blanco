#pragma once
#include "Core.h"
#include "Layer.h"

namespace Blanco 
{
    class BL_API LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Ref<Layer> layer);
        void PushOverLayer(Ref<Layer> layer);
        void PopLayer(Ref<Layer> layer);
        void PopOverLayer(Ref<Layer> layer);

        inline std::vector<Ref<Layer>>::iterator begin() { return m_LayerStack.begin(); }
        inline std::vector<Ref<Layer>>::iterator end() { return m_LayerStack.end(); }
    private:
        std::vector<Ref<Layer>> m_LayerStack;
        unsigned int m_LayerInsertIndex;
    };
}