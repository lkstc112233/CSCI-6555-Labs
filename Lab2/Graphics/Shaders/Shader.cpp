#include <iostream>
#include <initializer_list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../../fileop/fileLoader.h"

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

void ShaderProgram::setValue(const char* key, int i0) {
	glProgramUniform1i(getProgramId(), glGetUniformLocation(getProgramId(), key), i0);
}
// void ShaderProgram::setValue(const char* key, int i0, int i1);
// void ShaderProgram::setValue(const char* key, int i0, int i1, int i2);
// void ShaderProgram::setValue(const char* key, int i0, int i1, int i2, int i3);
// void ShaderProgram::setValue(const char* key, unsigned u0);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1, unsigned u2);
// void ShaderProgram::setValue(const char* key, unsigned u0, unsigned u1, unsigned u2, unsigned u3);
void ShaderProgram::setValue(const char* key, float f0) {
	glProgramUniform1f(getProgramId(), glGetUniformLocation(getProgramId(), key), f0);
}
// void ShaderProgram::setValue(const char* key, float f0, float f1);
// void ShaderProgram::setValue(const char* key, float f0, float f1, float f2);
// void ShaderProgram::setValue(const char* key, float f0, float f1, float f2, float f3);
// void ShaderProgram::setVector(const char* key, const glm::vec1& vec);
// void ShaderProgram::setVector(const char* key, const glm::vec2& vec);
void ShaderProgram::setVector(const char* key, const glm::vec3& vec) {
    glProgramUniform3fv(getProgramId(), glGetUniformLocation(getProgramId(), key), 1, glm::value_ptr(vec));
}
void ShaderProgram::setVector(const char* key, const glm::vec4& vec) {
    glProgramUniform4fv(getProgramId(), glGetUniformLocation(getProgramId(), key), 1, glm::value_ptr(vec));
}
// void ShaderProgram::setMatrix(const char* key, const glm::mat2& mat);
void ShaderProgram::setMatrix(const char* key, const glm::mat3& mat) {
    glProgramUniformMatrix3fv(getProgramId(), glGetUniformLocation(getProgramId(), key), 1, GL_FALSE, glm::value_ptr(mat));
}
void ShaderProgram::setMatrix(const char* key, const glm::mat4& mat) {
    glProgramUniformMatrix4fv(getProgramId(), glGetUniformLocation(getProgramId(), key), 1, GL_FALSE, glm::value_ptr(mat));
}