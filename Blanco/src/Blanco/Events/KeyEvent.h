#pragma once
#include "Blanco/Core.h"
#include "Event.h"

namespace Blanco {
	class BL_API KeyEvent :public Event {
	public:
		EVENT_CATEGORY(EventInput | EventKeyboard)

		inline int GetKeyCode() const{ return m_KeyCode; }
	protected:
		KeyEvent(int KeyCode) : m_KeyCode(KeyCode){}
		int m_KeyCode;
	};

	class BL_API KeyPressedEvent :public KeyEvent {
	public:
		KeyPressedEvent(int KeyCode, int RepeatCounts) :KeyEvent(KeyCode), m_RepeatCounts(RepeatCounts) {}
	
		EVENT_CLASS_TYPE(KeyPressed)

		std::string ToString() const override {
			std:: stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode << "(repeat" << m_RepeatCounts << "times)";
			return ss.str();
		}
	private:
		int m_RepeatCounts;

	};

	class BL_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(int KeyCode) :KeyEvent(KeyCode){}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode ;
			return ss.str();
		}
	};
}
