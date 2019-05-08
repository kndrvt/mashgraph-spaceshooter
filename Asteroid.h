#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"


//glm::vec3 Positions[] = {
//        glm::vec3( 1.0f,  2.0f,  5.0f),
//        glm::vec3( 2.0f,  5.0f, 15.0f),
//        glm::vec3(-1.5f, -2.2f, 20.5f),
//        glm::vec3(-3.8f, -2.0f, 12.3f),
//        glm::vec3( 2.4f, -0.4f, 30.5f),
//        glm::vec3(-1.7f,  3.0f, 7.5f),
//        glm::vec3( 1.3f, -2.0f, 20.5f),
//        glm::vec3( 1.5f,  2.0f, 20.5f),
//        glm::vec3( 1.5f,  0.2f, 10.5f),
//        glm::vec3(-1.3f,  1.0f, 10.5f)
//};


class Asteroid: public Model {
    GLfloat scl;
public:
    bool destroyed = false;
    glm::vec3 Pos = glm::vec3(1.0f, 1.0f, 100.0f);
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 5.0f;

    Asteroid(std::string dir): Model(dir) {}

    void draw(ShaderProgram shader, Camera camera, GLfloat currentFrame) {
        shader.StartUseShader();

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        glm::mat4 model(1.0);
        model = glm::translate(model, this->Pos);
        model = glm::rotate(model, - 0.5f * currentFrame, glm::vec3(1.0f, 1.0f, 0.0f) + this->Center);
        scl = 0.1f;
        model = glm::scale(model, glm::vec3(scl, scl, scl));
        shader.SetUniform("model", model);

        this->Draw(shader);
        shader.StopUseShader();
    }

    void movement(GLfloat deltaTime) {
        Pos += Speed * deltaTime * Front;
    }

    void reboot() {
        glm::vec2 tmp(random_range(10, 5), random_range(10, 5));
        tmp *= 0.01f;
        this->Pos = glm::vec3(tmp, 100.0f);
//      this->Front = glm::normalize(glm::vec3(tmp, -1.0f));
        this->destroyed = false;
    }
};
