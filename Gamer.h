#pragma once

#include "common.h"
#include "Camera.h"
#include "Bullet.h"
#include "SOIL.h"

GLfloat vertices[] = {
        // aim
        0.1f,  0.1f, 0.1f, 1.0f, 0.0f,
        0.1f, -0.1f, 0.1f, 1.0f, 1.0f,
        -0.1f, -0.1f, 0.1f, 0.0f, 1.0f,
        -0.1f,  0.1f, 0.1f, 0.0f, 0.0f,
        // health
        -2.2f, 0.95f, 0.1f, 0.0f, 0.0f,
        -2.2f, 0.85f, 0.1f, 0.0f, 1.0f,
        -2.1f, 0.95f, 0.1f, 0.1f, 0.0f,
        -2.1f, 0.85f, 0.1f, 0.1f, 1.0f,
        -2.1f, 0.95f, 0.1f, 0.0f, 0.0f,
        -2.1f, 0.85f, 0.1f, 0.0f, 1.0f,
        -2.0f, 0.95f, 0.1f, 0.1f, 0.0f,
        -2.0f, 0.85f, 0.1f, 0.1f, 1.0f,
        -2.0f, 0.95f, 0.1f, 0.0f, 0.0f,
        -2.0f, 0.85f, 0.1f, 0.0f, 1.0f,
        -1.9f, 0.95f, 0.1f, 0.1f, 0.0f,
        -1.9f, 0.85f, 0.1f, 0.1f, 1.0f,
        // scores
        1.9f, 0.95f, 0.1f, 0.0f, 0.0f,
        1.9f, 0.85f, 0.1f, 0.0f, 1.0f,
        2.0f, 0.95f, 0.1f, 0.1f, 0.0f,
        2.0f, 0.85f, 0.1f, 0.1f, 1.0f,
        2.0f, 0.95f, 0.1f, 0.0f, 0.0f,
        2.0f, 0.85f, 0.1f, 0.0f, 1.0f,
        2.1f, 0.95f, 0.1f, 0.1f, 0.0f,
        2.1f, 0.85f, 0.1f, 0.1f, 1.0f,
        2.1f, 0.95f, 0.1f, 0.0f, 0.0f,
        2.1f, 0.85f, 0.1f, 0.0f, 1.0f,
        2.2f, 0.95f, 0.1f, 0.1f, 0.0f,
        2.2f, 0.85f, 0.1f, 0.1f, 1.0f,
        // icon
        -2.2f, 0.95f, 0.1f, 0.0f, 0.0f,
        -2.2f, 0.85f, 0.1f, 0.0f, 1.0f,
        -2.35f, 0.95f, 0.1f, 1.0f, 0.0f,
        -2.35f, 0.85f, 0.1f, 1.0f, 1.0f,
        // ship
        2.0f,  1.0f, 0.1f, 1.0f, 0.0f,
        2.0f, -1.0f, 0.1f, 1.0f, 1.0f,
        -2.0f, -1.0f, 0.1f, 0.0f, 1.0f,
        -2.0f,  1.0f, 0.1f, 0.0f, 0.0f
};

GLint indices[] = {
        // aim
        0, 1, 2,
        2, 3, 0,
        // health
        4, 5, 6,
        5, 6, 7,
        8, 9, 10,
        9, 10, 11,
        12, 13, 14,
        13, 14, 15,
        // scores
        16, 17, 18,
        17, 18, 19,
        20, 21, 22,
        21, 22, 23,
        24, 25, 26,
        25, 26, 27,
        // icon
        28, 29, 30,
        29, 30, 31,
        // ship
        32, 33, 34,
        33, 34, 35

};

const int MAX_BULLET = 10;

class Gamer {
    unsigned int aimTexture;
    unsigned int digitsTexture;
    unsigned int heartTexture;
    unsigned int shipTexture;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int num_bullet = 0;

    void Init() {
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

        int width, height;
        unsigned char *image;

        // aim texture
        //
        glGenTextures(1, &aimTexture);
        glBindTexture(GL_TEXTURE_2D, aimTexture);
        image = SOIL_load_image("../textures/aim.png", &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        // digits texture
        //
        glGenTextures(1, &digitsTexture);
        glBindTexture(GL_TEXTURE_2D, digitsTexture);
        image = SOIL_load_image("../textures/digits.png", &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        // heart texture
        //
        glGenTextures(1, &heartTexture);
        glBindTexture(GL_TEXTURE_2D, heartTexture);
        image = SOIL_load_image("../textures/heart.png", &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        // ship texture
        //
        glGenTextures(1, &shipTexture);
        glBindTexture(GL_TEXTURE_2D, shipTexture);
        image = SOIL_load_image("../textures/ship.png", &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Draw(ShaderProgram shader, Camera camera) {

        glm::mat4 model(1.0);
        model = glm::scale(model, glm::vec3((float)camera.HEIGHT / (float)camera.WIDTH, 1.0, 1.0));
        shader.SetUniform("model", model);

        glm::mat4 model_digits(1.0);
        model_digits = glm::scale(model_digits, glm::vec3(190.0 / 326.0, 1.0, 1.0));
        shader.SetUniform("model_digits", model_digits);

        glBindVertexArray(VAO);

        // draw aim
        //
        glBindTexture(GL_TEXTURE_2D, aimTexture);

        shader.SetUniform("number", 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // draw health
        //
        glBindTexture(GL_TEXTURE_2D, digitsTexture);

        shader.SetUniform("number", 2);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        shader.SetUniform("number", 3);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        shader.SetUniform("number", 4);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        // draw scores
        //
        shader.SetUniform("number", 5);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

        shader.SetUniform("number", 6);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        shader.SetUniform("number", 7);
        glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);

        // draw heart icon
        //
        glBindTexture(GL_TEXTURE_2D, heartTexture);

        shader.SetUniform("number", 8);
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

        // draw ship
        //
        if (camera.SHIP) {
            glBindTexture(GL_TEXTURE_2D, shipTexture);

            shader.SetUniform("number", 9);
            glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }

public:
    GLfloat Radius = 1.0f;
    GLint Health = 100;
    GLint  Scores = 0;
    vector<Bullet> bullets;
    bool EndGame = false;

    Gamer(Camera camera) {
        for (int i = 0; i < MAX_BULLET; ++i) {
            bullets.push_back(Bullet(camera.Pos, glm::vec3(0.0f)));
            bullets[i].Color = glm::vec3(0.1 * i, 0.0, 0.5 + 0.05 * i);
            bullets[i].Speed = 100.0f;
        }
        Init();
    }

    ~Gamer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, Camera camera, GLfloat deltaTime) {
        shader_bullet.StartUseShader();
        for (int i = 0; i < bullets.size(); ++i) {
            this->bullets[i].movement(deltaTime);
            this->bullets[i].draw(shader_bullet, camera);
        }
        shader_bullet.StopUseShader();

        shader.StartUseShader();
        shader.SetUniform("Health", this->Health);
        shader.SetUniform("Scores", this->Scores);
        this->Draw(shader, camera);
        shader.StopUseShader();
    }

    void shoot(Camera camera) {
        bullets[num_bullet++].update(camera.Pos, camera.Front);
        if (num_bullet == bullets.size()) num_bullet = 0;
    }

    void hit(Camera camera, int num) {
        this->Scores += 10;
        this->bullets[num].update(camera.Pos, glm::vec3(0.0f));
    }

    void damage() {
        this->Health -= 20;
        if (this->Health <= 0) {
            this->EndGame = true;
        }
    }
};