#pragma once

#include "common.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "SOIL.h"

class Skybox {

public:
    Skybox(std::string dir) {
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        std::vector<std::string> faces = {
                dir + "right.png",
                dir + "left.png",
                dir + "top.png",
                dir + "bot.png",
                dir + "front.png",
                dir + "back.png"
        };

        this->cubemapTexture = loadCubemap(faces);

        GLfloat vertices[] = {
                5.0f, 5.0f, 5.0f,   // Верхний правый
                5.0f, -5.0f, 5.0f,   // Нижний правый
                -5.0f, -5.0f, 5.0f,   // Нижний левый
                -5.0f, 5.0f, 5.0f,   // Верхний левый
                5.0f, 5.0f, -5.0f,  // Верхний правый
                5.0f, -5.0f, -5.0f,   // Нижний правый
                -5.0f, -5.0f, -5.0f,  // Нижний левый
                -5.0f, 5.0f, -5.0f,    // Верхний левый
        };
        GLuint indices[] = {
                0, 1, 2, // Передняя грань
                2, 3, 0,
                4, 5, 6, // Задняя грань
                6, 7, 4,
                4, 5, 1, // Правая боковая грань
                1, 0, 4,
                7, 6, 2, // Левая боковая грань
                2, 3, 7,
                6, 5, 1, // Верхняя грань
                1, 2, 6,
                7, 4, 0, // Нижняя грань
                0, 3, 7
        };
        GL_CHECK_ERRORS;
        glBindVertexArray(skyboxVAO);

        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    ~Skybox() {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxEBO);
        glDeleteBuffers(1, &skyboxVBO);
    }

    void Draw() {
        glDepthMask(GL_FALSE);
        GL_CHECK_ERRORS;
        glBindVertexArray(skyboxVAO);
        GL_CHECK_ERRORS;
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        GL_CHECK_ERRORS;
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        GL_CHECK_ERRORS;
        glDepthMask(GL_TRUE);
        GL_CHECK_ERRORS;
        glBindVertexArray(0);
        GL_CHECK_ERRORS;
    }
    void draw(ShaderProgram shader, Camera camera, GLfloat currentFrame) {
        shader.StartUseShader();

        glm::mat4 view(1.0);
        view = camera.GetViewMatrix();
        shader.SetUniform("view", view);

        glm::mat4 proj(1.0);
        proj = camera.GetPerspectiveMatrix();
        shader.SetUniform("proj", proj);

        shader.SetUniform("time", currentFrame);
        this->Draw();
        shader.StopUseShader();
    }

private:
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint skyboxEBO;
    unsigned int cubemapTexture;

    unsigned int loadCubemap(std::vector<std::string> faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                SOIL_free_image_data(data);
            } else {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                SOIL_free_image_data(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
};