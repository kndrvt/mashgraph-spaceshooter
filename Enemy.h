#pragma once

#include "common.h"
#include "Model.h"
#include "Bullet.h"

const GLint maxHealth = 20;

vector<glm::vec3> enemyPositions = {
        glm::vec3(-10.0f, -10.0f, 250.0f),
        glm::vec3(-10.0f, -6.0f, 230.0f),
        glm::vec3(-10.0f, -2.0f, 210.0f),
        glm::vec3(-10.0f, 2.0f, 210.0f),
        glm::vec3(-10.0f, 6.0f, 230.0f),
        glm::vec3(-10.0f, 10.0f, 250.0f),
        glm::vec3(-6.0f, -10.0f, 230.0f),
        glm::vec3(-6.0f, -6.0f, 210.0f),
        glm::vec3(-6.0f, -2.0f, 190.0f),
        glm::vec3(-6.0f, 2.0f, 190.0f),
        glm::vec3(-6.0f, 6.0f, 210.0f),
        glm::vec3(-6.0f, 10.0f, 230.0f),
        glm::vec3(-2.0f, -10.0f, 210.0f),
        glm::vec3(-2.0f, -6.0f, 190.0f),
        glm::vec3(-2.0f, -2.0f, 170.0f),
        glm::vec3(-2.0f, 2.0f, 170.0f),
        glm::vec3(-2.0f, 6.0f, 190.0f),
        glm::vec3(-2.0f, 10.0f, 210.0f),
        glm::vec3(2.0f, -10.0f, 210.0f),
        glm::vec3(2.0f, -6.0f, 190.0f),
        glm::vec3(2.0f, -2.0f, 170.0f),
        glm::vec3(2.0f, 2.0f, 170.0f),
        glm::vec3(2.0f, 6.0f, 190.0f),
        glm::vec3(2.0f, 10.0f, 210.0f),
        glm::vec3(6.0f, -10.0f, 230.0f),
        glm::vec3(6.0f, -6.0f, 210.0f),
        glm::vec3(6.0f, -2.0f, 190.0f),
        glm::vec3(6.0f, 2.0f, 190.0f),
        glm::vec3(6.0f, 6.0f, 210.0f),
        glm::vec3(6.0f, 10.0f, 230.0f),
        glm::vec3(10.0f, -10.0f, 250.0f),
        glm::vec3(10.0f, -6.0f, 230.0f),
        glm::vec3(10.0f, -2.0f, 210.0f),
        glm::vec3(10.0f, 2.0f, 210.0f),
        glm::vec3(10.0f, 6.0f, 230.0f),
        glm::vec3(10.0f, 10.0f, 250.0f)
};


class Enemy: public Model {
    GLfloat scl;
    GLfloat rtt;
    GLfloat deathTime;
public:
    bool dead = false;
    glm::vec3 Pos = enemyPositions[(int)rand() % enemyPositions.size()];
    glm::vec3 Front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    GLfloat Speed = 10.0f;
    Bullet bullet;
    GLint Health = maxHealth;

    Enemy(std::string dir, GLfloat s = 0.5, GLfloat r = 0.0): Model(dir), scl(s), rtt(r) {
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
        this->Radius *= scl;
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime, GLfloat currentFrame) {
        shader.StartUseShader();

        glm::mat4 model(1.0);
        model = glm::mat4(1.0);
        model = glm::translate(model, Pos);
        model = glm::rotate(model, glm::radians(rtt), glm::vec3(0.0, 1.0, 0.0));
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
        if ((glm::length(this->Pos - bullet.Pos) >= 30.0f) || (glm::length(bullet.Front) == 0))
            bullet.update(this->Pos, glm::vec3(0.0f));
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
        bullet.update(Pos, glm::normalize(camera.Pos - Pos));
    }

    void damage(GLfloat currentFrame) {
        this->Health -= 20;
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
        this->Pos = enemyPositions[(int)rand() % enemyPositions.size()];
        this->dead = false;
    }

    void hit() {
        this->bullet.update(this->Pos, glm::vec3(0.0f));
    }

};

