#pragma once

#include"Base.h"
#include"Dependencies.h"
#include<functional>



namespace vista 
{
	class Window
	{
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Window();
		~Window();

		void initWindow(int height, int width, const char* name);

		GLFWwindow* getGLFWwindowPointer();

		inline void setEventCallback(const EventCallbackFn& callback) { m_Data.eventCallback = callback; }

		//void glfwWindowCloseCallback(GLFWwindow* window);

		void update();

		void destroyWindow();

	private:
		GLFWwindow* m_GlfwWindow;

		struct WindowData
		{
			std::string wTitle;
			int height, width;
			bool vSync;

			EventCallbackFn eventCallback;
		};
		WindowData m_Data;

	};
}

