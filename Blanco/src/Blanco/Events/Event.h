#pragma once
#include "BLpch.h"
#include "Blanco/Core/Core.h"

namespace Blanco {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFoucus, WindowLostFocus,WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed,KeyReleased,KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;}\
	                           virtual EventType GetEventType() const override {return GetStaticType();}\
	                           virtual const char* GetEventName() const override {return #type;}
#define EVENT_CATEGORY(category) virtual int GetEventCategoryFlag() const override {return category;}

	class Event {
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual int GetEventCategoryFlag() const = 0;
		virtual std::string ToString() const { return GetEventName(); }

		bool IsInCategory(EventCategory category) const {
			return GetEventCategoryFlag() & category;
		}

		bool Handled = false;
	};

	class Dispatcher {
	public:
		Dispatcher(Event& event) :m_Event(event) {}

		template<typename T>
		bool Dispatch(std::function<bool(T&)> fnc) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = fnc(*((T*)&m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& event) {
		stream << event.ToString();
		return stream;
	}
}
