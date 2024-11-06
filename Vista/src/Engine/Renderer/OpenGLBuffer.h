#pragma once


#include "Dependencies.h"



namespace vista
{
	class OpenGLBuffer
	{
	public:
		virtual void createBuffer() = 0;
		virtual void getBufferData() = 0;
		virtual unsigned int& bufferObject() = 0;

		virtual ~OpenGLBuffer() {}
	};

	//
	class VertexBuffer : public OpenGLBuffer
	{
	public:
		VertexBuffer();
		virtual void createBuffer() override;
		virtual void getBufferData() override;
		inline virtual unsigned int& bufferObject() override { return VBO; }

	private:
		unsigned int VBO;
		float* vertices;
	};

	//
	class ElementBuffer : OpenGLBuffer
	{
	public:
		virtual void createBuffer() override;
		virtual void getBufferData() override;
		inline virtual unsigned int& bufferObject() override { return EBO; }

	private:
		unsigned int EBO;
		int* indexes;
	};

}



