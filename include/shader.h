#pragma once

#include <string>

class Shader {
public:

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	unsigned int getProgramHandle() const { return programHandle; }
private:
	unsigned int programHandle;

	void loadShader(const std::string& vertexPath, const std::string& fragmentPath);
};