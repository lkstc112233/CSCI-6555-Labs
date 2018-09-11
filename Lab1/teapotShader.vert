# version 410 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x - 0.7, aPos.z - 0.5, aPos.y, 1.0);
    TexCoord = vec2(aPos.xy);
}