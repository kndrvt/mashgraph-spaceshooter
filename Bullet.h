#pragma once

#include "common.h"
#include "Model.h"
#include "Camera.h"

class Bullet : public Model {
    GLfloat scl = 0.01;
public:
    glm::vec3 Pos;
    glm::vec3 Front;
    glm::vec3 Color;
    GLfloat Speed = 200.0f;

    Bullet(glm::vec3 p = glm::vec3(0.0, 0.0, 0.0), glm::vec3 t = glm::vec3(0.0, 0.0, 0.0)) : Model(
            std::string("../objects/planet/planet.obj")), Pos(p), Front(t) {
        this->Radius *= scl;
    }

    void draw(ShaderProgram shader, Camera camera) {
        shader.StartUseShader();

        glm::mat4 model(1.0);
        model = glm::translate(model, Pos);
        model = glm::scale(model, glm::vec3(scl, scl, scl));
        shader.SetUniform("model", model);

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        shader.SetUniform("Color", Color);
        this->Draw(shader);

        shader.StopUseShader();
    }

    void update(glm::vec3 p, glm::vec3 t) {
        Pos = p;
        Front = t;
    }

    void movement(GLfloat deltaTime) {
        Pos += Speed * deltaTime * Front;
    }

};