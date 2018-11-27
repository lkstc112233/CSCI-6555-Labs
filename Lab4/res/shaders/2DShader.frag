# version 410 core
out vec4 FragColor;

uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform float opacity = 1.0;

void main()
{
    FragColor = vec4(color, opacity);
}
