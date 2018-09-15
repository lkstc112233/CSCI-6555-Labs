# version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec4 color;
out vec2 TexCoord;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    color = vec4(in_color, 1.0);
    TexCoord = aTexCoord;
}