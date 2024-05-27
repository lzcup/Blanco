#pragma once
#include "Blanco/Core.h"
#include "Blanco/Layer.h"
#include "Blanco/Events/ApplicationEvent.h"
#include "Blanco/Events/KeyEvent.h"
#include "Blanco/Events/MouseEvent.h"

namespace Blanco 
{
    class BL_API ImguiLayer:public Layer{
    public:
        ImguiLayer();
        ~ImguiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
    private:
        float m_Time;
    private:
        bool OnImguiWindowResize(WindowResizeEvent& event);
        bool OnImguiKeyPressed(KeyPressedEvent& event);
        bool OnImguiKeyReleased(KeyReleasedEvent& event);
        bool OnImguiKeyTyped(KeyTypedEvent& event);
        bool OnImguiMouseButtonPressed(MouseButtonPressedEvent& event);
        bool OnImguiMouseButtonReleased(MouseButtonReleasedEvent& event);
        bool OnImguiMouseMoved(MouseMovedEvent& event);
        bool OnImguiMouseScrolled(MouseScrolledEvent& event);
    };
}