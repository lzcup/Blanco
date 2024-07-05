#pragma once
#include "Blanco/Core/Core.h"
#include "Blanco/Core/Layer.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/Events/KeyEvent.h"
#include "Blanco/Events/MouseEvent.h"

namespace Blanco 
{
    class ImguiLayer:public Layer{
    public:
        ImguiLayer();
        ~ImguiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        virtual void OnImguiRender() override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }
        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
    };
}