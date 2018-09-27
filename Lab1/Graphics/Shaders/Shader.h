#ifndef GRAPHICS_SHADERS_SHADER_H
#define GRAPHICS_SHADERS_SHADER_H

#include <initializer_list>
#include <glm/glm.hpp>

/**
 * A class holding a compiled shader program.
 */
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
 * A shader program, containing vertex/geomentory/fragment shaders, and has a set of functions 
 * managing uniform variables.
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
    void setVector(const char* key, const glm::vec1& vec);
    void setVector(const char* key, const glm::vec2& vec);
    void setVector(const char* key, const glm::vec3& vec);
    void setVector(const char* key, const glm::vec4& vec);
    void setMatrix(const char* key, const glm::mat2& mat);
    void setMatrix(const char* key, const glm::mat3& mat);
    void setMatrix(const char* key, const glm::mat4& mat);
    void use();
};

#endif // GRAPHICS_SHADERS_SHADER_H