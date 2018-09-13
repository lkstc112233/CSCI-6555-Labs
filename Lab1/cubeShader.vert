# version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

out vec2 TexCoord;
out vec3 color;

void main()
{
    // vec3 pos = aPos / 6;
    vec3 pos = aPos;
    // gl_Position = transform * vec4(pos.x - 0.4, pos.yz, 1.0);
    gl_Position = transform * vec4(pos, 1.0);
    TexCoord = vec2(pos.xy);
    color = pos + 0.5;
    // color = abs(vec3(pos.x - 0.4, pos.yz));
}