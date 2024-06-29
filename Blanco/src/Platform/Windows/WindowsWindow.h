#pragma once
#include "Blanco/Core/Window.h"
#include "GLFW/glfw3.h"
#include "Blanco/Renderer/GraphicsContext.h"

namespace Blanco {
	class WindowsWindow :public Window {
	public:
		WindowsWindow(const WindowProp& prop);
		~WindowsWindow();

		virtual void Update() override;

		virtual inline uint32_t GetWidth() const override { return m_Data.m_Width; }
		virtual inline uint32_t GetHeight() const override { return m_Data.m_Height; }

		virtual inline void* GetNativeWindow() const override { return m_Window; }

		virtual void SetEventCallBack(const EventCallBackFnc& fnc) override { m_Data.m_Fnc = fnc; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

	private:
		virtual void Init(const WindowProp& prop);
		virtual void ShutDown();

	private:
		struct WindowData
		{
			uint32_t m_Width = 0, m_Height = 0;
			std::string m_Title;
			bool m_VSync = true;
			EventCallBackFnc m_Fnc;
		};

		WindowData m_Data;
		GLFWwindow* m_Window;
		Ref<GraphicsContext> m_Context;
	};
}