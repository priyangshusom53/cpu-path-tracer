#include "Camera.h"

namespace vista
{
	Camera::Camera()
		:cameraPos(0.0f, 3.0f, 3.0f),
		 cameraFront(0.0f, 0.0f, -1.0f),
		 cameraUp(0.0f, 1.0f, 0.0f),
		 view(glm::mat4(1.0f)),
		 projection(glm::mat4(1.0f)),
		 deltaTime(0.0f),
		 lastFrame(0.0f)
	{
		
	}
	glm::mat4 Camera::getViewMat()
	{
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		return view;
	}
	glm::mat4 Camera::getProjMat()
	{
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
			100.0f);
		return projection;
	}

	void Camera::processInput(GLFWwindow* window)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
			cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
			cameraSpeed;
		
	}

	void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

}

