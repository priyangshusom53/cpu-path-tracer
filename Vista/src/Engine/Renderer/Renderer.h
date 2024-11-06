#pragma once


#include "Engine/Base.h"
#include "Dependencies.h"
#include "OpenGLShader.h"
#include "OpenGLBuffer.h"
#include "Engine/Layer.h"
#include "Engine/Scene/Camera.h"

namespace vista
{
	class Renderer : public Layer
	{
	public:
		Renderer();
		void initRenderer();
		void render(GLFWwindow* window);

		void onUpdate() override;
		Camera* cam;

	private:
		openglShader *shader;

		unsigned int VAO;

		VertexBuffer* vertexbuffer;
		ElementBuffer* elementBuffer;

		unsigned int texture;
		glm::mat4 model;
		glm::mat4 MVP;
		glm::vec2 coordShift;
		
	};
}

