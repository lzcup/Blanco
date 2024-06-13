#pragma once
#include "Blanco/Core/Core.h"
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
		KeyPressedEvent(int KeyCode, int ScanCode, int RepeatCounts) :KeyEvent(KeyCode), m_ScanCode(ScanCode), m_RepeatCounts(RepeatCounts) {}
	
		EVENT_CLASS_TYPE(KeyPressed)

		inline int GetScanCode() const { return m_ScanCode; }
		inline int GetRepeatCounts() const { return m_RepeatCounts; }

		std::string ToString() const override {
			std:: stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode << "(repeat" << m_RepeatCounts << "times)";
			return ss.str();
		}
	private:
		int m_RepeatCounts;
		int m_ScanCode;
	};

	class BL_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(int KeyCode, int ScanCode) :KeyEvent(KeyCode), m_ScanCode(ScanCode) {}

		EVENT_CLASS_TYPE(KeyReleased)

		inline int GetScanCode() const { return m_ScanCode; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode ;
			return ss.str();
		}
	private:
		int m_ScanCode;
	};

	class BL_API KeyTypedEvent :public KeyEvent {
	public:
		KeyTypedEvent(int KeyCode) :KeyEvent(KeyCode) {}

		EVENT_CLASS_TYPE(KeyTyped)

			std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();
		}
	};
}
