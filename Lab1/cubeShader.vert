# version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 color;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    TexCoord = vec2(aPos.xy);
    color = aPos + 0.5;
}