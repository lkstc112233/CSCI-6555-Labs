#ifndef SHADERS_SHADER_H
#define SHADERS_SHADER_H

#include <initializer_list>

class Shader
{
  private:
    unsigned int shaderId = 0;
    bool valid = true;

  public:
    Shader(Shader &&toMove)
    {
        shaderId = toMove.shaderId;
        toMove.shaderId = 0;
    }
    virtual ~Shader();
    int getShaderId() const { return shaderId; }
    bool isValid() const { return valid; }
    static Shader createVertexShader(const char *filename);
    static Shader createFragmentShader(const char *filename);

  private:
    Shader(const char *filename, int shaderType, const char *errorHint);
};

/**
 * Usage: ShaderProgram program{shaders...};
 * */
class ShaderProgram
{
  private:
    int programId;
    bool valid = true;

  public:
    ShaderProgram(std::initializer_list<Shader> list);
    int getProgramId() const { return programId; }
    bool isValid() const { return valid; }
    void use();
};

#endif // SHADERS_SHADER_H