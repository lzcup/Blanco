#pragma once
#include "Blanco/Core/Core.h"
#include "Event.h"

namespace Blanco {
	class KeyEvent :public Event {
	public:
		EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

		inline int GetKeyCode() const{ return m_KeyCode; }
	protected:
		KeyEvent(int KeyCode) : m_KeyCode(KeyCode){}
		int m_KeyCode;
	};

	class KeyPressedEvent :public KeyEvent {
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

	class KeyReleasedEvent :public KeyEvent {
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

	class KeyTypedEvent :public KeyEvent {
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
