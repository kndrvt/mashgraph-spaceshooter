#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 Color;
uniform sampler2D texture_diffuse1;

void main()
{
    vec4 col1 = vec4(Color, 0.5f);
    vec4 col2 = texture(texture_diffuse1, TexCoords);
    FragColor = mix(col1, col2, 0.5 * (col1 + col2));
}