#pragma once


#include "Dependencies.h"




namespace vista
{
	class Camera
	{
	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		glm::mat4 view;
		glm::mat4 projection;

		float deltaTime;
		float lastFrame;

		bool firstMouse = true;
		double lastX = 400.0f;
		double lastY = 300.0f;
		double yaw	= -90.0f;
		double pitch = 0.0f;
	public:
		Camera();
		glm::mat4 getViewMat();
		glm::mat4 getProjMat();
		void processInput(GLFWwindow* window);
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	
	};

}

