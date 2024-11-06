#include "openglBuffer.h"


namespace vista
{
	//Vertex Buffer
	VertexBuffer::VertexBuffer()
		:VBO(0), vertices(nullptr)
	{
		
	}

	void VertexBuffer::createBuffer()
	{
		
		/*glGenBuffers(1, &VBO);
		float vertices[32] = {
			// positions		// colors			// texture coords
			0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
			0.5f,-0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		   -0.5f,-0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		   -0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f  // top left

		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

		glGenBuffers(1, &VBO);

		float vertices[18] = {
			 10.0f, 0.0f, -10.0f,
			-10.0f, 0.0f,  10.0f,
			-10.0f, 0.0f, -10.0f,
			 10.0f, 0.0f, -10.0f,
			 10.0f, 0.0f,  10.0f,
			-10.0f, 0.0f,  10.0f,
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void VertexBuffer::getBufferData()
	{
		

	}

	//Element Buffer
	void ElementBuffer::createBuffer()
	{
		/*glGenBuffers(1, &EBO);
		unsigned int indices[] = {
						0, 1, 3,
						1, 2, 3
		};


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	}

	void ElementBuffer::getBufferData()
	{

	}
}
