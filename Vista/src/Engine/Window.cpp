#include "Window.h"



namespace vista 
{

	
	//
	Window::Window()
		:m_GlfwWindow(nullptr)
	{

	}

	//
	void Window::initWindow(int height, int width, const char* name) 
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_GlfwWindow = glfwCreateWindow(height, width, name, NULL, NULL);
		if (m_GlfwWindow == NULL)
		{
			printf("Failed to create GLFW window\n");
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_GlfwWindow);
		glfwSwapInterval(0);
		glfwSetWindowUserPointer(m_GlfwWindow, &m_Data);

		//GLFW callbacks
		glfwSetWindowSizeCallback(m_GlfwWindow, [](GLFWwindow* window, int height, int width)
		{
				glViewport(0, 0, height, width);
		});

		glfwSetWindowFocusCallback(m_GlfwWindow, [](GLFWwindow* window, int focused)
		{

		});

		glfwSetFramebufferSizeCallback(m_GlfwWindow, [](GLFWwindow* window, int height, int width)
		{

		});

		glfwSetWindowCloseCallback(m_GlfwWindow, [](GLFWwindow* window)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;

				data.eventCallback(event);

		});
		glfwSetCursorPosCallback(m_GlfwWindow, [](GLFWwindow* window, double xpos, double ypos) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event(xpos, ypos);
				data.eventCallback(event);

		});
		glfwSetInputMode(m_GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}

	//
	void Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_GlfwWindow);
	}

	//
	GLFWwindow* Window::getGLFWwindowPointer()
	{
		return m_GlfwWindow;
	}

	//
	void Window::destroyWindow()
	{
		glfwDestroyWindow(m_GlfwWindow);
		glfwTerminate();
	}

	//
	Window::~Window()
	{

	}


}
