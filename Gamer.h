#pragma once

#include "common.h"
#include "Camera.h"
#include "Bullet.h"

class Gamer {
    void Draw(ShaderProgram shader) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
// Устанавливаем настройки фильтрации и преобразований (на текущей текстуре)

// Загружаем и генерируем текстуру
        int width, height;
        unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
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
        bullet.Speed = 50.0f;
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime) {
        shader.StartUseShader();
        shader.SetUniform("Health", this->Health);
        shader.SetUniform("Scores", this->Scores);
//        this->Draw(shader);
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