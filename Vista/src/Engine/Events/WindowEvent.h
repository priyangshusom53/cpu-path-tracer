#pragma once

#include "Event.h"
#include "Engine/Base.h"

namespace vista
{
	//
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}
		EVENT_CLASS_TYPE(WindowClose)
	};

	//
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int height, int width)
			:m_Height(height), m_Width(width) {}

		inline int getHeight() { return m_Height; }
		inline int getWidth() { return m_Width; }

		EVENT_CLASS_TYPE(WindowResize);

	private:
		int m_Height, m_Width;
	};
}
