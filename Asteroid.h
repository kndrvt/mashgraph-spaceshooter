#pragma once

#include "common.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"


vector<glm::vec3> asteroidPositions = {
        glm::vec3(-8.0f, -8.0f, 230.0f),
        glm::vec3(-8.0f, -4.0f, 210.0f),
        glm::vec3(-8.0f, 0.0f, 190.0f),
        glm::vec3(-8.0f, 4.0f, 210.0f),
        glm::vec3(-8.0f, 8.0f, 230.0f),
        glm::vec3(-4.0f, -8.0f, 210.0f),
        glm::vec3(-4.0f, -4.0f, 190.0f),
        glm::vec3(-4.0f, 0.0f, 170.0f),
        glm::vec3(-4.0f, 4.0f, 190.0f),
        glm::vec3(-4.0f, 8.0f, 210.0f),
        glm::vec3(0.0f, -8.0f, 190.0f),
        glm::vec3(0.0f, -4.0f, 170.0f),
        glm::vec3(0.0f, 0.0f, 150.0f),
        glm::vec3(0.0f, 4.0f, 170.0f),
        glm::vec3(0.0f, 8.0f, 190.0f),
        glm::vec3(4.0f, -8.0f, 210.0f),
        glm::vec3(4.0f, -4.0f, 190.0f),
        glm::vec3(4.0f, 0.0f, 170.0f),
        glm::vec3(4.0f, 4.0f, 190.0f),
        glm::vec3(4.0f, 8.0f, 210.0f),
        glm::vec3(8.0f, -8.0f, 230.0f),
        glm::vec3(8.0f, -4.0f, 210.0f),
        glm::vec3(8.0f, 0.0f, 190.0f),
        glm::vec3(8.0f, 4.0f, 210.0f),
        glm::vec3(8.0f, 8.0f, 230.0f)
};

class Asteroid: public Model {
    GLfloat scl;
    GLfloat destructionTime;
public:
    bool destroyed = false;
    glm::vec3 Pos = asteroidPositions[(int)rand() % asteroidPositions.size()];
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 5.0f;

    Asteroid(std::string dir, GLfloat s = 0.5f): Model(dir), scl(s) {
        this->Radius *= this->scl;
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

        shader.SetUniform("time", currentFrame);
        shader.SetUniform("time0", this->destructionTime);
        shader.SetUniform("destroyed", this->destroyed);
        this->Draw(shader);
        shader.StopUseShader();
    }

    void movement(GLfloat deltaTime) {
        if (this->Pos.z < 0.0f) {
            this->reboot();
        }
        Pos += Speed * deltaTime * Front;
    }

    void destruction(GLfloat currentFrame) {
        this->destructionTime = currentFrame;
        this->destroyed = true;
    }

    void reboot() {
        this->Pos = asteroidPositions[(int)rand() % asteroidPositions.size()];
        this->destroyed = false;
    }
};
