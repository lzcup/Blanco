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

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImguiRender() override;

        void Begin();
        void End();
    };
}