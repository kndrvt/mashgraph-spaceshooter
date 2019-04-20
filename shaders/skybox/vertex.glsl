#version 330

layout(location = 0) in vec3 skybox_vertex;

out vec3 skybox_coords;

uniform mat4 view;
uniform mat4 proj;

void main(void) {
  gl_Position  = (proj * mat4(mat3(view)) * vec4(skybox_vertex, 1.0));
  skybox_coords = skybox_vertex;
}
