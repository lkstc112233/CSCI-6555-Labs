# version 410 core
in vec2 TexCoord;
in vec3 color;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float ratio;

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), ratio) * vec4(color, 1.0);
}