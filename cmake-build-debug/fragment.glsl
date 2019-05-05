//#version 330 core
//
//in vec2 our_tex_coords;
//in vec3 our_vertex;
//
//out vec4 color;
//
//uniform sampler2D our_texture;
//
//float eps = 0.001;
//
//void main() {
//  vec2 texcord = vec2(0);
//  if(abs(our_vertex.x) >= 0.5 - eps) texcord = our_vertex.yz;
//  if(abs(our_vertex.y) >= 0.5 - eps) texcord = our_vertex.xz;
//  if(abs(our_vertex.z) >= 0.5 - eps) texcord = our_vertex.xy;
//  texcord += 0.5;
//  color = texture(our_texture, texcord);
//}