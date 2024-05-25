#pragma once
#include "Blanco/Core.h"
#include "Event.h"

namespace Blanco {
	class BL_API WindowCloseEvent :public Event {
	public:
		WindowCloseEvent(){}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class BL_API WindowResizeEvent :public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :m_Width(width), m_Height(height) {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent:(" << m_Width << "," << m_Height << ")";
			return ss.str();
		}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
	private:
		unsigned int m_Width, m_Height;
	};

	class BL_API WindowFoucusEvent :public Event {
	public:
		WindowFoucusEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowFoucus)
	};

	class BL_API WindowLostFocusEvent :public Event {
	public:
		WindowLostFocusEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowLostFocus)
	};

	class BL_API WindowMovedEvent :public Event {
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

	class BL_API AppTickEvent :public Event {
	public:
		AppTickEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};

	class BL_API AppUpdateEvent :public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class BL_API AppRenderEvent :public Event {
	public:
		AppRenderEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}