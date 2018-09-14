# version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 color;
out vec3 norm;
out vec3 worldCord;

void main()
{
    norm = normalize(mat3(transpose(inverse(transform))) * aNormal);
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    worldCord = vec3(transform * vec4(aPos, 1.0));
    TexCoord = vec2(aPos.xy);
    color = aPos + 0.5;
}