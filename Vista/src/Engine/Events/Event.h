#pragma once


#include <string>
#include <functional>

namespace vista {



	//
	enum class EventType
	{

		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled

	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType(){ return EventType::##type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

	class Event
	{

		friend class EventDispatcher;

	public:
		virtual EventType getEventType() const = 0;

		virtual const char* getName() const = 0;

		virtual std::string toString() const { return getName(); }


	public:
		bool handled = false;
	};

	class EventDispatcher
	{

		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& e)
			: event(e)
		{
		}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.handled = func(*(T*)&event);
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};
}

