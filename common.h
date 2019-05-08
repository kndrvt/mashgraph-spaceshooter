#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <cstdlib>
#include <ctime>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//полезный макрос для проверки ошибок
//в строчке, где он был записан вызывает ThrowExceptionOnGLError, которая при возникновении ошибки opengl
//пишет в консоль номер текущей строки и название исходного файла
//а также тип ошибки
#define GL_CHECK_ERRORS ThrowExceptionOnGLError(__LINE__,__FILE__);
#define pi 3.14159265358979323846f;

static void ThrowExceptionOnGLError(int line, const char *file) {

    static char errMsg[512];

    //вызывается функция glGetError, проверяющая не произошла ли ошибка
    //в каком-то вызове opengl и если произошла, то какой код у ошибки
    GLenum gl_error = glGetError();

    if (gl_error == GL_NO_ERROR)
        return;

    switch (gl_error) {
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM file " << file << " line " << line << std::endl;
            break;

        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE file " << file << " line " << line << std::endl;
            break;

        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION file " << file << " line " << line << std::endl;
            break;

        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW file " << file << " line " << line << std::endl;
            break;

        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW file " << file << " line " << line << std::endl;
            break;

        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY file " << file << " line " << line << std::endl;
            break;

        case GL_NO_ERROR:
            break;

        default:
            std::cerr << "Unknown error @ file " << file << " line " << line << std::endl;
            break;
    }

    if (gl_error != GL_NO_ERROR)
        throw std::runtime_error(errMsg);
}
