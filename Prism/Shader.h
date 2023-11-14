#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//program id
	unsigned int ID;

	//constructor
	Shader(const char* vertexPath, const char* fragmentPath);
	// activate the shader program
	void Activate() const;
	static void Deactivate();
	// delete the shader program
	void Delete();
	// set uniform values
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;
	int GetUniformLocation(const std::string& name) const;

private:
	std::string readShaderCodeFile(const char* shaderPath);
	void checkShaderCompilationSuccess(unsigned int shader, const char* shaderType);
	void checkProgramLinkSuccess();
};

#endif
