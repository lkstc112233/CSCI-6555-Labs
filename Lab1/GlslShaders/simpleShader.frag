# version 410 core
in vec4 color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float ratio;

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), ratio) * color;
}