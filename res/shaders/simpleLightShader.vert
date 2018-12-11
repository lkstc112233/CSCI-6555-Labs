#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 norm;
out vec3 worldCord;

void main() {
  norm = normalize(mat3(transpose(inverse(transform))) * aNorm);
  gl_Position = projection * view * transform * vec4(aPos, 1.0);
  worldCord = vec3(transform * vec4(aPos, 1.0));
  color = clamp(vec4(1 - aPos, 1.0), 0, 1);
}