#version 330 core

in vec3 skybox_coords;

out vec4 color;

uniform samplerCube skybox;

void main() {
  color = texture(skybox, skybox_coords);
}