#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform int Health;
uniform int Scores;
uniform sampler2D texAim;
//uniform sampler2D texHealth;
//uniform sampler2D texScores;
//uniform sampler2D texShip;

void main()
{
	vec4 color = texture(texAim, TexCoords);
	if (color.w == 1.0) color.w = 0.5;
	FragColor = color;
}