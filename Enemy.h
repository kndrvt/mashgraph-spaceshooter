#pragma once

#include "common.h"
#include "Model.h"
#include "Bullet.h"

const GLint maxHealth = 40;

glm::vec3 enemyPositions[] = {
        glm::vec3( 0.0f,  0.0f, 125.0f),
        glm::vec3( 5.0f,  2.0f, 165.0f),
        glm::vec3(-2.2f, -1.5f, 120.5f),
        glm::vec3(-2.0f, -3.8f, 142.3f),
        glm::vec3( 0.4f, -2.4f, 130.5f),
        glm::vec3(-3.0f,  1.7f, 137.5f),
        glm::vec3( 2.0f, -1.3f, 140.5f),
        glm::vec3( 2.0f,  1.5f, 150.5f),
        glm::vec3( 0.2f,  1.5f, 130.5f),
        glm::vec3(-1.0f,  1.3f, 180.5f)
};


class Enemy: public Model {
    GLfloat scl = 0.5;
    GLfloat deathTime;
public:
    bool dead = false;
    glm::vec3 Pos = enemyPositions[(int)rand() % 10];
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 10.0f;
    Bullet bullet;
    GLint Health = maxHealth;

    Enemy(std::string dir): Model(dir) {
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
        this->Radius *= scl;
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime, GLfloat currentFrame) {
        shader.StartUseShader();

        glm::mat4 model(1.0);
        model = glm::mat4(1.0);
        model = glm::translate(model, Pos);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(scl));
        shader.SetUniform("model", model);

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        shader.SetUniform("time", currentFrame);
        shader.SetUniform("time0", this->deathTime);
        shader.SetUniform("dead", this->dead);
        this->Draw(shader);

        shader.StopUseShader();

        shader_bullet.StartUseShader();
        this->bullet.movement(deltaTime);
        this->bullet.draw(shader_bullet, camera);
        shader_bullet.StopUseShader();
    }

    void movement(GLfloat deltaTime) {
        if (this->Pos.z < 0.0f) {
            this->reboot();
        }
        Pos += Speed * deltaTime * Front;
    }

    void shoot(Camera camera) {
        bullet.update(Pos, camera.Pos - Pos);
    }

    void damage(GLfloat currentFrame) {
        this->Health -= 40;
        if (this->Health <= 0) {
            this->death(currentFrame);
        }
    }

    void death(GLfloat currentFrame) {
        this->Health = maxHealth;
        this->deathTime = currentFrame;
        this->dead = true;
    }

    void reboot() {
        this->Pos = enemyPositions[(int)rand() % 10];
        this->dead = false;
    }

};

