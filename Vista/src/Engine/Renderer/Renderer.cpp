#include "Renderer.h"
#include<iostream>
#include "stb_image.h"

namespace vista
{
	Renderer::Renderer()
		:VAO(0), texture(0)
	{

		shader = new openglShader(VISTA_SHADER_NAME("coordSys.vert"), VISTA_SHADER_NAME("coordSys.frag"));
		vertexbuffer = new VertexBuffer();
		elementBuffer = new ElementBuffer();
		cam = new Camera();
		MVP = glm::mat4(1.0);
		
	}
	
	void Renderer::initRenderer()
	{
		

		//
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		
		vertexbuffer->createBuffer();
		//elementBuffer->createBuffer();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
		/*
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			(void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);*/



		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		//texture
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("C:\\dev\\_vista\\Vista\\Vista\\assests\\nightSky.jpg", &width, &height,
			&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}


		const GLubyte* ver = glGetString(GL_VENDOR |GL_VERSION);
		std::cout << reinterpret_cast<const char*>(ver);

		model = glm::rotate(model, glm::radians(-90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));
		coordShift = glm::vec2(0.05f, 0.0f);

	}

	void Renderer::render(GLFWwindow* window)
	{

		shader->use();
		//draw triangles
		cam->processInput(window);
		MVP = cam->getProjMat() * cam->getViewMat();
		int modelLoc = glGetUniformLocation(shader->shaderProgramID, "MVP");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(MVP));
		int coordShiftLoc = glGetUniformLocation(shader->shaderProgramID, "coordShift");
		
		glUniform2fv(coordShiftLoc, 1, glm::value_ptr(coordShift));
		coordShift += glm::vec2(0.05f, 0.0f);


		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL),
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void Renderer::onUpdate()
	{
		//render();
	}
}