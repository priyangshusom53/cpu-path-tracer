#pragma once

#include"Event.h"

namespace vista
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y)
			:xPos(x), yPos(y) {}
		float inline getXPos() { return xPos; }
		float inline getYPos() { return yPos; }

		EVENT_CLASS_TYPE(MouseMoved);

	private:
		double xPos;
		double yPos;
	};
}
