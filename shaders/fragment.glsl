#version 330 core

in vec2 our_tex_coord;

out vec4 color;

uniform sampler2D our_texture;

void main() {
  color = texture(our_texture, our_tex_coord);
}