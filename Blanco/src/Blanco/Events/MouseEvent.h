#pragma once
#include "Blanco/Core/Core.h"
#include "Event.h"

namespace Blanco {
	class BL_API MouseButtonEvent :public Event {
	public:
		EVENT_CATEGORY(EventInput | EventCategoryMouseButton)

		inline int GetMouseCode() const { return m_MouseCode; }
	protected:
		MouseButtonEvent(int MouseCode) : m_MouseCode(MouseCode) {}
		int m_MouseCode;
	};

	class BL_API MouseButtonPressedEvent :public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int MouseCode) :MouseButtonEvent(MouseCode) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << m_MouseCode ;
			return ss.str();
		}
	};

	class BL_API MouseButtonReleasedEvent :public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int MouseCode) :MouseButtonEvent(MouseCode) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent:" << m_MouseCode ;
			return ss.str();
		}
	};

	class BL_API MouseMovedEvent :public Event {
	public:
		MouseMovedEvent(float x, float y) :m_X(x), m_Y(y) {}

		EVENT_CATEGORY(EventInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMoved)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent:(" << m_X << "," << m_Y << ")";
			return ss.str();
		}

		inline float GetX() const { return m_X; }
		inline float GetY() const { return m_Y; }
	private:
		float m_X, m_Y;
	};

	class BL_API MouseScrolledEvent :public Event {
	public:
		MouseScrolledEvent(float offsetX, float offsetY) :m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		EVENT_CATEGORY(EventInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseScrolled)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent:(" << m_OffsetX << "," << m_OffsetY << ")";
			return ss.str();
		}

		inline float GetXOffset() const { return m_OffsetX; }
		inline float GetYOffset() const { return m_OffsetY; }
	private:
		float m_OffsetX, m_OffsetY;
	};
}