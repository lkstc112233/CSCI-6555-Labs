#ifndef SHADERS_SHADER_H
#define SHADERS_SHADER_H

#include <initializer_list>

class ShaderBase
{
  private:
    unsigned int shaderId = 0;
    bool valid = true;

  public:
    ShaderBase(ShaderBase &&toMove)
    {
        shaderId = toMove.shaderId;
        toMove.shaderId = 0;
    }
    virtual ~ShaderBase();
    int getShaderId() const { return shaderId; }
    bool isValid() const { return valid; }
    static ShaderBase createVertexShader(const char *filename);
    static ShaderBase createFragmentShader(const char *filename);

  private:
    ShaderBase(const char *filename, int shaderType, const char *errorHint);
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
    ShaderProgram(std::initializer_list<ShaderBase> list);
    int getProgramId() const { return programId; }
    bool isValid() const { return valid; }
    void use();
};

#endif // SHADERS_SHADER_H