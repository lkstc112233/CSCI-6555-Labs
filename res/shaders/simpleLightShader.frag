#version 410 core
in vec4 color;
in vec3 norm;
in vec3 worldCord;

out vec4 FragColor;

uniform float opacity = 1.0;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {
  vec3 lightColor = vec3(1.0, 1.0, 1.0);

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

  vec3 result = (ambient + specular + diffuse) * color.rgb;

  FragColor = vec4(result, color.a * opacity);
}