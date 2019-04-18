#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 color;

out vec3 our_color;

void main(void) {
  gl_Position  = vec4(vertex,1.0);
  our_color = color;
}
