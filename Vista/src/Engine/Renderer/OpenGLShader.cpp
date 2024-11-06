#include "OpenGLShader.h"

namespace vista
{


	openglShader::openglShader(char* vertexPath, char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		const char *vertexCode, *fragmentCode;
		
		FileImport vImporter(vertexPath);
		vertexCode = vImporter.readFile(FALSE);
		FileImport fImporter(fragmentPath);
		fragmentCode = fImporter.readFile(FALSE);

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexCode, NULL);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			CORE_LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n{0}\n", infoLog);
		}
		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentCode, NULL);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			CORE_LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n{0}\n", infoLog);
		}

		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertex);
		glAttachShader(shaderProgramID, fragment);
		glLinkProgram(shaderProgramID);
		// print linking errors if any
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			CORE_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED:\n{0}\n", infoLog);
		}
		// delete shaders; they’re linked into our program and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	openglShader::openglShader(char* computePath, char* vertexPath, char* fragmentPath)
	{
		const char* computeCode;

		FileImport vImporter(vertexPath);
		computeCode = vImporter.readFile(FALSE);

		unsigned int compute;
		int success;
		char infoLog[512];
		// vertex Shader
		compute = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(compute, 1, &computeCode, NULL);
		glCompileShader(compute);
		// print compile errors if any
		glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(compute, 512, NULL, infoLog);
			CORE_LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n{0}\n", infoLog);
		}

		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, compute);
		glLinkProgram(shaderProgramID);
		// print linking errors if any
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			CORE_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED:\n{0}\n", infoLog);
		}
		// delete shaders; they’re linked into our program and no longer necessary
		glDeleteShader(compute);
	}

	void openglShader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void openglShader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void openglShader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void openglShader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
	}
	void openglShader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void openglShader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
	}
	void openglShader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void openglShader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
	}
	void openglShader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void openglShader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void openglShader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void openglShader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	char* openglShader::getShaderPath(const char* sName)
	{
		const char* pathName = "C:\\dev\\_vista\\Vista\\Vista\\src\\shader\\";
		size_t result_size = strlen(pathName) + strlen(sName) + 1;
		char* result = new char[result_size];
		strcpy_s(result, result_size, pathName);
		strcat_s(result, result_size, sName);
		return result;
	}

}
