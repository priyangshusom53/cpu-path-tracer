#pragma once


#include "Base.h"
#include "Dependencies.h"

namespace vista
{
	class Layer
	{
	public:

		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& e) {}

		inline const std::string& getName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};

}

