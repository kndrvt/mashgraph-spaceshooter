#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include <string>

glm::vec3 Positions[] = {
        glm::vec3( 1.0f,  2.0f,  5.0f),
        glm::vec3( 2.0f,  5.0f, 15.0f),
        glm::vec3(-1.5f, -2.2f, 20.5f),
        glm::vec3(-3.8f, -2.0f, 12.3f),
        glm::vec3( 2.4f, -0.4f, 30.5f),
        glm::vec3(-1.7f,  3.0f, 7.5f),
        glm::vec3( 1.3f, -2.0f, 20.5f),
        glm::vec3( 1.5f,  2.0f, 20.5f),
        glm::vec3( 1.5f,  0.2f, 10.5f),
        glm::vec3(-1.3f,  1.0f, 10.5f)
};


class Asteroid: public Model {
public:

    Asteroid(std::string dir): Model(dir) {}

    void draw(ShaderProgram shader, Camera camera) {
        shader.StartUseShader();

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        glm::mat4 model(1.0);
        for (int i = 0; i < 10; ++i) {
            model = glm::mat4(1.0);
            model = glm::translate(model, Positions[i]);
            model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            float tmp = 0.01 * i;
            model = glm::scale(model, glm::vec3(tmp, tmp, tmp));
            shader.SetUniform("model", model);
            this->Draw(shader);
        }
        shader.StopUseShader();

    }

private:
};
