#pragma once
#include "BLpch.h"
#include "Core.h"
#include "Events/Event.h"

namespace Blanco {

	struct WindowProp {
		WindowProp(unsigned int width = 1280, unsigned int height = 720, const std::string& title = "Blanco Engine")
			:m_Width(width), m_Height(height), m_Title(title) {}

		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Title;
	};


	class BL_API Window {
	public:
		using EventCallBackFnc = std::function<void(Event& event)>;
		virtual ~Window() {};

		virtual void Update() = 0;

		virtual inline unsigned int GetWidth() const = 0;
		virtual inline unsigned int GetHeight() const = 0;

		virtual inline void* GetNativeWindow() const = 0;

		//window attribute
		virtual void SetEventCallBack(const EventCallBackFnc& fnc) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProp& prop = WindowProp());
	};
}


