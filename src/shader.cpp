#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>

#include "shader.h"

void Shader::Compile(
	const char* vertexSource,
	const char* fragSource,
	const char* geoSource)
{
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexSource, NULL);
	glCompileShader(vShader);
	LogError(vShader, "Vertex Shader");

	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragSource, NULL);
	glCompileShader(fShader);
	LogError(fShader, "Fragment Shader");

	unsigned int gShader {};
	if (geoSource != nullptr) {
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geoSource, NULL);
		glCompileShader(gShader);
		LogError(gShader, "Geometry Shader");
	}

	m_SID = glCreateProgram();
	glAttachShader(m_SID, vShader);
	glAttachShader(m_SID, fShader);

	if (geoSource != nullptr) {
		glAttachShader(m_SID, gShader);
	}

	glLinkProgram(m_SID);
	LogError(m_SID, "Binary");

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	if (geoSource != nullptr) {
		glDeleteShader(gShader);
	}
}

void Shader::LogError(unsigned int id, const std::string_view& name)
{
	int  success {};
	char logInfo[512];

	if (name.data() != "Binary") {
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 512, NULL, logInfo);
			std::cout << "Error! " << name << " compilation failed\n"
					  << logInfo << std::endl;
		}
	}
	else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, NULL, logInfo);
			std::cout << "Error! " << name << " linking failed\n"
					  << logInfo << std::endl;
		}
	}
}
