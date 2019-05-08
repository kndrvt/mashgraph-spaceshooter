#pragma  once

#include "common.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 Pos;
    glm::vec3 Front;
    GLfloat Yaw;
    GLfloat Pitch;

    Camera(GLsizei W, GLsizei H) : WIDTH(W), HEIGHT(H) {
        Yaw = 90.0f;
        Pitch = 0.0f;
        FOV = 45.0f;
        Pos = glm::vec3(0.0f, 0.0f, 3.0f);
        Front = glm::vec3(0.0f, 0.0f, 1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        WorldUp = Up;
        MovementSpeed = 20.0f;
        MouseSensitivity = 0.1f;
    }

    void begin() {
        Yaw = 90.0f;
        Pitch = 0.0f;
        FOV = 45.0f;
        Pos = glm::vec3(0.0f, 0.0f, 3.0f);
        Front = glm::vec3(0.0f, 0.0f, 1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
    }

    glm::mat4 GetPerspectiveMatrix() {
        return glm::perspective(glm::radians(FOV), (GLfloat) WIDTH / HEIGHT, 0.1f, 200.0f);
    }

    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Pos += this->Front * velocity;
        if (direction == BACKWARD)
            this->Pos -= this->Front * velocity;
        if (direction == LEFT)
            this->Pos -= this->Right * velocity;
        if (direction == RIGHT)
            this->Pos += this->Right * velocity;
        if (direction == UP)
            this->Pos += this->Up * velocity;
        if (direction == DOWN)
            this->Pos -= this->Up * velocity;
    }

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, bool GodMode) {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
        if (!GodMode) {
            if (this->Yaw > 179.0f)
                this->Yaw = 179.0f;
            if (this->Yaw < 1.0f)
                this->Yaw = 1.0f;
        }

        this->update();
    }

    void ProcessMouseScroll(GLfloat yoffset) {
        if (FOV >= 30.0f && FOV <= 45.0f)
            FOV -= yoffset;
        if (FOV <= 30.0f)
            FOV = 30.0f;
        if (FOV >= 45.0f)
            FOV = 45.0f;
    }

private:
    GLfloat FOV;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    glm::vec3 Right;
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLsizei WIDTH;
    GLsizei HEIGHT;

    void update() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
