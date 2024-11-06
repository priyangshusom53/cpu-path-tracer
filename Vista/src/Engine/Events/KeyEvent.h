#pragma once


#include"Event.h"

namespace vista {

	//
	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode () const { return m_KeyCode; }

	protected:
		KeyEvent(int keyCode)
		:m_KeyCode(keyCode) {}

		int m_KeyCode;

	};

	//
	class KeyPressedEvent : public KeyEvent
	{

	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			:KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int getRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	//
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			:KeyEvent(keyCode){}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
