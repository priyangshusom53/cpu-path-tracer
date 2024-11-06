#include "InitGlad.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include<stdio.h>

void InitGlad::initGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
	}

}