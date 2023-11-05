#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vShaderCodeStr = readShaderCodeFile(vertexPath);
	std::string fShaderCodeStr = readShaderCodeFile(fragmentPath);
	const char* vShaderCode = vShaderCodeStr.c_str();
	const char* fShaderCode = fShaderCodeStr.c_str();

	// compile shaders
	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkShaderCompilationSuccess(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkShaderCompilationSuccess(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkProgramLinkSuccess();

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Activate() const
{
	glUseProgram(ID);
}

void Shader::Deactivate()
{
	glUseProgram(0);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(ID, name.c_str());
}

std::string Shader::readShaderCodeFile(const char* shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << shaderPath << "\n" << e.what() << std::endl;
	}

	return shaderCode;
}

void Shader::checkShaderCompilationSuccess(unsigned int shader, const char* shaderType)
{
	int success;
	char infoLog[512];
	// check shader compilation success
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// output compilation error log if failed
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << "\n" << std::endl;
	}
}

void Shader::checkProgramLinkSuccess()
{
	int success;
	char infoLog[512];
	// check program linking success
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// output linking error log if failed
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n" << std::endl;
	}
}