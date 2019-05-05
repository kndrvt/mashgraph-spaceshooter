#pragma once

#include "Model.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <string>

class Bullet : public Model {
public:
    glm::vec3 Pos;
    glm::vec3 Target;
    glm::vec3 Color;
    GLfloat Speed = 15.0f;

    Bullet(glm::vec3 p = glm::vec3(0.0, 0.0, 0.0), glm::vec3 t = glm::vec3(0.0, 0.0, 0.0)) : Model(
            std::string("../objects/planet/planet.obj")), Pos(p), Target(t) {}

    Bullet(Bullet &b) : Model(std::string("../objects/planet/planet.obj")) {
        this->Pos = b.Pos;
        this->Target = b.Target;
        this->Speed = b.Speed;
    }

    void draw(ShaderProgram shader, Camera camera) {
        glm::mat4 model(1.0);
        model = glm::translate(model, Pos);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        shader.SetUniform("model", model);

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        shader.SetUniform("Color", Color);
        this->Draw(shader);
    }

    void update(glm::vec3 p, glm::vec3 t) {
        Pos = p;
        Target = t;
    }

    void movement(GLfloat deltaTime) {
        Pos += Speed * deltaTime * Target;
    }

};