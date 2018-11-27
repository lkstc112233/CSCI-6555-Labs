# version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

out vec2 TexCoord;
out vec3 color;

void main()
{
    gl_Position = transform * vec4(aPos.x - 0.4, aPos.yz, 1.0);
    TexCoord = vec2(aPos.xy);
    color = vec3(aPos.x - 0.4, aPos.yz);
}