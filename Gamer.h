#pragma once

#include "common.h"
#include "Camera.h"
#include "Bullet.h"

class Gamer {
public:
    GLfloat Radius = 0.5f;
    GLint Health = 100;
    GLint  Scores = 0;
    Bullet bullet;
    bool EndGame = false;

    Gamer(Camera camera) {
        bullet.update(camera.Pos, glm::vec3(0.0f));
        bullet.Color = glm::vec3(0.8, 0.0, 1.0);
        bullet.Speed = 50.0f;
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime) {
        shader_bullet.StartUseShader();
        this->bullet.movement(deltaTime);
        this->bullet.draw(shader_bullet, camera);
        shader_bullet.StopUseShader();
    }

    void shoot(Camera camera) {
        bullet.update(camera.Pos, camera.Front);
    }

    void hit(Camera camera) {
        this->Scores += 100;
        this->bullet.update(camera.Pos, glm::vec3(0.0f));
    }

    void damage() {
        this->Health -= 20;
        if (this->Health <= 0) {
            this->EndGame = true;
        }
    }
};