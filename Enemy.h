#pragma once

#include "Model.h"
#include "Bullet.h"
#include <string>

class Enemy: public Model {
public:
    bool dead;
    glm::vec3 Pos;
    GLfloat Speed;
    Bullet bullet;

    Enemy(std::string dir): Model(dir) {
        dead = false;
        Pos = glm::vec3(0.0f, 0.0f, -100.0f);
        Speed = 15.0f;
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
    }

    void movement(GLfloat deltaTime) {
        Pos.z += Speed * deltaTime;
    }

    void shoot() {
        bullet.update(Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - Pos));
        bullet.Color = glm::vec3(1.0, 0.0, 0.0);
    }

};

