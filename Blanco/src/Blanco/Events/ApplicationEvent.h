#pragma once
#include "Blanco/Core/Core.h"
#include "Event.h"

namespace Blanco {
	class WindowCloseEvent :public Event {
	public:
		WindowCloseEvent(){}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent :public Event {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) :m_Width(width), m_Height(height) {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent:(" << m_Width << "," << m_Height << ")";
			return ss.str();
		}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowFoucusEvent :public Event {
	public:
		WindowFoucusEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowFoucus)
	};

	class WindowLostFocusEvent :public Event {
	public:
		WindowLostFocusEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowLostFocus)
	};

	class WindowMovedEvent :public Event {
	public:
		WindowMovedEvent(float x, float y) :m_X(x), m_Y(y) {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowMoved)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMoved:(" << m_X << "," << m_Y << ")";
			return ss.str();
		}

		inline float GetX() const { return m_X; }
		inline float GetY() const { return m_Y; }
	private:
		float m_X, m_Y;
	};

	class AppTickEvent :public Event {
	public:
		AppTickEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};

	class AppUpdateEvent :public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class AppRenderEvent :public Event {
	public:
		AppRenderEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}