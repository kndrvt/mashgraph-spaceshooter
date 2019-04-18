#version 330 core

in vec3 our_color;
in vec2 our_tex_coord;

out vec4 color;

uniform our_texture;

void main() {
//  color = vec4(our_color, 1.0f);
  color = texture(our_texture, our_tex_coord);
}