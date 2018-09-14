# version 410 core
in vec2 TexCoord;
in vec3 norm;
in vec3 color;
in vec3 worldCord;

uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);

    vec3 lightDir = normalize(lightPos - worldCord);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * vec3(color.x >=0.49 ? 1.0 : 0, color.y >=0.49 ? 1.0 : 0, color.z >= 0.49 ? 1.0 : 0);
    FragColor = vec4(result, 1.0);
}