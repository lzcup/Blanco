#pragma once
#include "Blanco\Window.h"
#include "GLFW/glfw3.h"

namespace Blanco {
	class BL_API WindowsWindow :public Window {
	public:
		WindowsWindow(const WindowProp& prop);
		~WindowsWindow();

		void Update() override;

		inline unsigned int GetWidth() const override;
		inline unsigned int GetHeight() const override;

		void SetEventCallBack(EventCallBackFnc&) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

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

		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}