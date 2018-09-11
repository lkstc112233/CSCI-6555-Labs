#ifndef SHADERS_SHADER_H
#define SHADERS_SHADER_H

#include <initializer_list>
#include <glm/glm.hpp>

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
    void setValue(const char* key, int i0);
    void setValue(const char* key, int i0, int i1);
    void setValue(const char* key, int i0, int i1, int i2);
    void setValue(const char* key, int i0, int i1, int i2, int i3);
    void setValue(const char* key, unsigned u0);
    void setValue(const char* key, unsigned u0, unsigned u1);
    void setValue(const char* key, unsigned u0, unsigned u1, unsigned u2);
    void setValue(const char* key, unsigned u0, unsigned u1, unsigned u2, unsigned u3);
    void setValue(const char* key, float f0);
    void setValue(const char* key, float f0, float f1);
    void setValue(const char* key, float f0, float f1, float f2);
    void setValue(const char* key, float f0, float f1, float f2, float f3);
    void setMatrix(const char* key, const glm::mat2& mat);
    void setMatrix(const char* key, const glm::mat3& mat);
    void setMatrix(const char* key, const glm::mat4& mat);
    void use();
};

#endif // SHADERS_SHADER_H