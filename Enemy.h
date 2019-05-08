#pragma once

#include "common.h"
#include "Model.h"
#include "Bullet.h"

const GLint maxHealth = 40;

GLfloat random_range(int end, int begin) {
    if ((int) rand() % 2) return (int) rand() % end + begin;
    else return -((int) rand() % end + begin);
}

class Enemy: public Model {
    GLfloat scl = 0.5;
public:
    bool dead = false;
    glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 100.0f);
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 20.0f;
    Bullet bullet;
    GLint Health = maxHealth;

    Enemy(std::string dir): Model(dir) {
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
        this->Radius *= scl;
    }

    void draw(ShaderProgram shader, Camera camera) {
        shader.StartUseShader();

        glm::mat4 model(1.0);
        model = glm::mat4(1.0);
        model = glm::translate(model, Pos);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(scl, scl, scl));
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

    void hit() {
        this->Health -= 20;
        if (this->Health <= 0) {
            this->dead = true;
            this->Health = maxHealth;
        }
    }

    void reboot() {
        glm::vec2 tmp(random_range(10, 0), random_range(10, 0));
        tmp *= 0.01f;
        this->Pos = glm::vec3(tmp, 100.0f);
//                enemy.Front = glm::normalize(glm::vec3(tmp, -1.0f));
        this->dead = false;
    }

};

