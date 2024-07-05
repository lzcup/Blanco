#pragma once
#include "BLpch.h"
#include "Blanco/Core/Core.h"
#include "Blanco/Events/Event.h"

namespace Blanco {

	struct WindowProp {
		WindowProp(const std::string& title = "Blanco Engine", uint32_t width = 1600, uint32_t height = 900)
			:m_Title(title), m_Width(width), m_Height(height) {}

		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
	};


	class Window {
	public:
		using EventCallBackFnc = std::function<void(Event& event)>;
		virtual ~Window() {};

		virtual void Update() = 0;

		virtual inline uint32_t GetWidth() const = 0;
		virtual inline uint32_t GetHeight() const = 0;

		virtual inline void* GetNativeWindow() const = 0;

		//window attribute
		virtual void SetEventCallBack(const EventCallBackFnc& fnc) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Ref<Window> Create(const WindowProp& prop = WindowProp());
	};
}


