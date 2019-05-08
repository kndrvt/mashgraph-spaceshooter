#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"


glm::vec3 asteroidPositions[] = {
        glm::vec3( 0.0f,  0.0f, 125.0f),
        glm::vec3( 2.0f,  5.0f, 165.0f),
        glm::vec3(-1.5f, -2.2f, 120.5f),
        glm::vec3(-3.8f, -2.0f, 132.3f),
        glm::vec3( 2.4f, -0.4f, 130.5f),
        glm::vec3(-1.7f,  3.0f, 117.5f),
        glm::vec3( 1.3f, -2.0f, 140.5f),
        glm::vec3( 1.5f,  2.0f, 150.5f),
        glm::vec3( 1.5f,  0.2f, 155.5f),
        glm::vec3(-1.3f,  1.0f, 180.5f)
};


class Asteroid: public Model {
    GLfloat scl = 0.5f;
public:
    glm::vec3 Pos = asteroidPositions[(int)rand() % 10];
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 10.0f;

    Asteroid(std::string dir): Model(dir) {
        this->Radius *= scl;
    }

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
        model = glm::rotate(model, - 0.5f * currentFrame, glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scl));
        shader.SetUniform("model", model);

        this->Draw(shader);
        shader.StopUseShader();
    }

    void movement(GLfloat deltaTime) {
        if (this->Pos.z < 0.0f) {
            this->reboot();
        }
        Pos += Speed * deltaTime * Front;
    }

    void reboot() {
        this->Pos = asteroidPositions[(int)rand() % 10];
    }

    void destruction() {
        this->reboot();

    }
};
