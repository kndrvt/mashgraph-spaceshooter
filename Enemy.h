#pragma once

#include "Model.h"
#include "Bullet.h"
#include <string>

class Enemy: public Model {
public:
    bool dead;
    glm::vec3 Pos;
    glm::vec3 Front;
    GLfloat Speed;
    Bullet bullet;

    Enemy(std::string dir): Model(dir) {
        dead = false;
        Pos = glm::vec3(0.0f, 0.0f, -100.0f);
        Front = glm::vec3(0.0f, 0.0f, 1.0f);
        Speed = 15.0f;
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
    }

    void draw(ShaderProgram shader, Camera camera) {
        shader.StartUseShader();

        glm::mat4 model(1.0);
        model = glm::mat4(1.0);
        model = glm::translate(model, Pos);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
        shader.SetUniform("model", model);

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        shader.SetUniform("dead", dead);
        this->Draw(shader);

        shader.StopUseShader();
    }

    void movement(GLfloat deltaTime) {
        Pos += Speed * deltaTime * Front;
    }

    void shoot() {
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
    }

};

