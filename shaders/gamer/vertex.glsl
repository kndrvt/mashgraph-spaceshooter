#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 model_digits;
uniform int number;

void main()
{
    mat4 res = model;
    if ((2 <= number) && (number <= 8)) {
        res = model_digits * model;
    }
    gl_Position = res * vec4(aPos, 1.0);
    TexCoords = aCoords;
}