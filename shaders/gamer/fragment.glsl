#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform int Health;
uniform int Score;
uniform int number;
uniform sampler2D tex;

vec2 dig(int n, int par) {
	int d;
	int m;
	vec2 tmp = TexCoords;
	if (number == n) {
		m = 100;
	} else if (number == n + 1) {
		m = 10;
	} else if (number == n + 2) {
		m = 1;
	}
	d = (par / m) % 10;
	tmp.x += d * 0.1;
	return tmp;
}

void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	if ((2 <= number) && (number <= 4)) {
		color = texture(tex, dig(2, Health));
	} else if ((5 <= number) && (number <= 7)) {
		color = texture(tex, dig(5, Score));
	} else {
		color = texture(tex, TexCoords);
	}
	if ((number == 1) && (color.w == 1.0)) color.w = 0.7;
	FragColor = color;
}