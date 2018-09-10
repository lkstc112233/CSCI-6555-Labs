# version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 aTexCoord;

out vec4 color;
out vec2 TexCoord;

void main()
{
    // gl_Position = vec4(aPos.x - 0.7, aPos.z - 0.5, aPos.y, 1.0);
    gl_Position = vec4(aPos, 1.0);
    color = vec4(in_color, 1.0);
    TexCoord = aTexCoord;
}