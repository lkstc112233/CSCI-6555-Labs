# version 410 core
layout (location = 0) in vec2 aPos;

uniform mat3 transform;
uniform float z;

void main()
{
    gl_Position = vec4((transform * vec3(aPos, 1.0)).xy, z, 1.0);
}