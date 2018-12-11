# version 410 core
in vec2 TexCoord;
in vec3 norm;
in vec3 color;
in vec3 worldCord;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);

    vec3 lightDir = normalize(lightPos - worldCord);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - worldCord);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + specular + diffuse) * vec3(color.x >=0.49 ? 1.0 : 0, color.y >=0.49 ? 1.0 : 0, color.z >= 0.49 ? 1.0 : 0);
    FragColor = vec4(result, 1.0);
}