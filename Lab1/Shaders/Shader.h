#ifndef SHADERS_SHADER_H
#define SHADERS_SHADER_H

class ShaderBase {
private:
    unsigned int shaderId = 0;
    bool valid = true;
public:
    virtual ~ShaderBase();
    int getShaderId() {return shaderId;}
    bool isValid() {return valid;}
protected:
    ShaderBase(const char* filename, int shaderType, const char* errorHint);
};

class VertexShader : public ShaderBase {
public:
    VertexShader(const char* filename);
};

class FragmentShader : public ShaderBase {
public:
    FragmentShader(const char* filename);
};

#endif // SHADERS_SHADER_H