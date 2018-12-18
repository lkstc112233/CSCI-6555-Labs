#version 410 core
in vec4 color;

out vec4 FragColor;

uniform float opacity = 0.3;

void main() { FragColor = vec4(color.xyz, color.w * opacity); }