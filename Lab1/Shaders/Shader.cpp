#include <iostream>
#include <initializer_list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "../fileop/fileLoader.h"

Shader::Shader(const char *filename, int shaderType, const char *errorHint)
{
    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, FileLoader(filename), NULL);
    glCompileShader(shaderId);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << errorHint << infoLog << std::endl;
        valid = false;
    }
}

Shader::~Shader()
{
    if (shaderId)
    {
        glDeleteShader(shaderId);
    }
}

Shader Shader::createVertexShader(const char *filename) {
    return Shader(filename, GL_VERTEX_SHADER, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
}

Shader Shader::createFragmentShader(const char *filename) {
    return Shader(filename, GL_FRAGMENT_SHADER, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> list)
{
    programId = glCreateProgram();
    for (auto &shader : list)
    {
        glAttachShader(programId, shader.getShaderId());
    }
    glLinkProgram(programId);

    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK::LINK_FAILED\n"
                  << infoLog << std::endl;
    }
}

void ShaderProgram::use()
{
    glUseProgram(programId);
}

class ShaderIdHolder {
private:
    GLint savedId;
public:
    ShaderIdHolder(unsigned newProgram) {
        glGetIntegerv(GL_CURRENT_PROGRAM, &savedId);
        glUseProgram(newProgram);
    }
    ~ShaderIdHolder() {
        glUseProgram(savedId);
    }
};

void ShaderProgram::setValue(const char* key, int i0) {
    ShaderIdHolder holder(getProgramId());
	glUniform1i(glGetUniformLocation(getProgramId(), key), i0);
}
// void ShaderProgram::setValue(const char* key, int i0, int i1);
// void ShaderProgram::setValue(const char* key, int i0, int i1, int i2);
// void ShaderProgram::setValue(const char* key, int i0, int i1, int i2, int i3);
// void ShaderProgram::setValue(const char* key, unsigned u0);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1, unsigned u2);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1, unsigned u2, unsigned u3);
void ShaderProgram::setValue(const char* key, float f0) {
    ShaderIdHolder holder(getProgramId());
	glUniform1f(glGetUniformLocation(getProgramId(), key), f0);
}
// void ShaderProgram::setValue(const char* key, float f0, float f1);
// void ShaderProgram::setValue(const char* key, float f0, float f1, float f2);
// void ShaderProgram::setValue(const char* key, float f0, float f1, float f2, float f3);