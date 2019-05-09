#pragma once

#include "common.h"
#include "Camera.h"
#include "Bullet.h"
#include "SOIL.h"

GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // Верхний правый
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // Нижний правый
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // Нижний левый
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // Верхний левый
};

GLint indices[] = {
        0, 1, 2,
        2, 3, 0
};

class Gamer {
    unsigned int texture;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void upload_texture() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char *image = SOIL_load_image("../textures/aim.png", &width, &height, nullptr, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Draw(ShaderProgram shader, Camera camera) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.SetUniform("aimTex", 0);

        glm::mat4 model(1.0);
        shader.SetUniform("model", model);

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
public:
    GLfloat Radius = 1.0f;
    GLint Health = 100;
    GLint  Scores = 0;
    Bullet bullet;
    bool EndGame = false;

    Gamer(Camera camera) {
        bullet.update(camera.Pos, glm::vec3(0.0f));
        bullet.Color = glm::vec3(0.8, 0.0, 1.0);
        bullet.Speed = 100.0f;
        upload_texture();
    }

    ~Gamer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime) {
        shader.StartUseShader();
        shader.SetUniform("Health", this->Health);
        shader.SetUniform("Scores", this->Scores);
        this->Draw(shader, camera);
        shader.StopUseShader();

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