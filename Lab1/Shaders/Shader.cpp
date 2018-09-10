#include <iostream>
#include <initializer_list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "../fileop/fileLoader.h"

ShaderBase::ShaderBase(const char *filename, int shaderType, const char *errorHint)
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

ShaderBase::~ShaderBase()
{
    if (shaderId)
    {
        glDeleteShader(shaderId);
    }
}

VertexShader::VertexShader(const char *filename)
    : ShaderBase(
          filename,
          GL_VERTEX_SHADER,
          "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") {}

FragmentShader::FragmentShader(const char *filename)
    : ShaderBase(
          filename,
          GL_FRAGMENT_SHADER,
          "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") {}

ShaderProgram::ShaderProgram(std::initializer_list<ShaderBase> list)
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