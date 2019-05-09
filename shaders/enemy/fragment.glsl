#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform bool dead;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

void main()
{
    if (dead) {
        FragColor = vec4(1.0f, 0.5f, 0.5f, 0.5f);
    } else {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
}