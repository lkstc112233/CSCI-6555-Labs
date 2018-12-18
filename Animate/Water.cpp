#include "Water.h"

Water::Water()
    : shader({Shader::createVertexShader("res/shaders/WaterShader.vert"),
              Shader::createFragmentShader("res/shaders/WaterShader.frag")}) {}

void Water::draw() {}