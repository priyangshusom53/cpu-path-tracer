#pragma once

#include "Engine/Base.h"
#include "Dependencies.h"
#include "FileImport.h"

namespace vista
{
	//
	class openglShader
	{

	public:
		// the program ID
		unsigned int shaderProgramID;
		// constructor reads and builds the shader
		openglShader(char* vertexPath, char* fragmentPath);
		openglShader(char* computePath, char* vertexPath, char* fragmentPath);
		// use/activate the shader
		inline void use(){ glUseProgram(shaderProgramID); }
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;

		static char* getShaderPath(const char* sName);

	};

}

#define VISTA_SHADER_NAME(X)  openglShader::getShaderPath(X)
