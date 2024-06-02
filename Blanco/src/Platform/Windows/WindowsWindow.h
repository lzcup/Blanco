#pragma once
#include "Blanco/Window.h"
#include "GLFW/glfw3.h"
#include "Blanco/Renderer/GraphicsContext.h"

namespace Blanco {
	class BL_API WindowsWindow :public Window {
	public:
		WindowsWindow(const WindowProp& prop);
		~WindowsWindow();

		virtual void Update() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.m_Width; }
		virtual inline unsigned int GetHeight() const override { return m_Data.m_Height; }

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
			unsigned int m_Width,m_Height;
			std::string m_Title;
			bool m_VSync;
			EventCallBackFnc m_Fnc;
		};

		WindowData m_Data;
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
	};
}