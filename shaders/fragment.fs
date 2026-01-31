#version 460 core
out vec4 FragColor;

in vec3 uniColor;

void main() {
  FragColor = vec4(uniColor, 1.0f);
}