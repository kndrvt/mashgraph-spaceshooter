//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "Model.h"

//External dependencies
#include <GLFW/glfw3.h>
#include "SOIL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <vector>

static const GLsizei WIDTH = 800, HEIGHT = 600; //размеры окна

bool keys[1024];
bool firstMouse = true;
GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat deltaTime = 0.0f;    // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;    // Время вывода последнего кадра

int initGL();
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void move();
unsigned int loadCubemap(std::vector<std::string> faces);

int main(int argc, char **argv) {

    if (!glfwInit()) {
        return -1;
    }

    //запрашиваем контекст opengl версии 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (initGL() != 0) {
        return -1;
    }

    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR) {
        gl_error = glGetError();
    }

    std::unordered_map<GLenum, std::string> shaders;

    shaders[GL_VERTEX_SHADER] = "../shaders/enemy/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/enemy/fragment.glsl";
    ShaderProgram shader_enemy(shaders);

    shaders[GL_VERTEX_SHADER] = "../shaders/skybox/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/skybox/fragment.glsl";
    ShaderProgram shader_skybox(shaders);

    glfwSwapInterval(1); // force 60 frames per second

//    //Создаем и загружаем геометрию поверхности
//    //
//    GLuint VBO;
//    GLuint EBO;
//    GLuint VAO;
//    GLuint texture;
//    {
//        GLfloat vertices[] = {
//                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Верхний правый
//                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // Нижний правый
//                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // Нижний левый
//                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // Верхний левый
//                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Верхний правый
//                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // Нижний правый
//                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,   // Нижний левый
//                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f    // Верхний левый
//        };
//        GLuint indices[] = {
//                0, 1, 2, // Передняя грань
//                2, 3, 0,
//                4, 5, 6, //Задняя грань
//                6, 7, 4,
//                4, 5, 1, // Правая боковая грань
//                1, 0, 4,
//                7, 6, 2, // Левая боковая грань
//                2, 3, 7,
//                6, 5, 1, //Верхняя грань
//                1, 2, 6,
//                7, 4, 0, //Нижняя грань
//                0, 3, 7
//        };
//
//        glGenBuffers(1, &VBO);
//        glGenBuffers(1, &EBO);
//        glGenVertexArrays(1, &VAO);
//        glGenTextures(1, &texture);
//
//        glBindTexture(GL_TEXTURE_2D, texture);
//        int width, height;
//        unsigned char *image = SOIL_load_image("../textures/space.jpg",
//                                               &width, &height, 0, SOIL_LOAD_RGB);
//        GL_CHECK_ERRORS;
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//        GL_CHECK_ERRORS;
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//        GL_CHECK_ERRORS;
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//        GL_CHECK_ERRORS;
//        glGenerateMipmap(GL_TEXTURE_2D);
//        GL_CHECK_ERRORS;
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        GL_CHECK_ERRORS;
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        GL_CHECK_ERRORS;
//        SOIL_free_image_data(image);
//        GL_CHECK_ERRORS;
//        glBindTexture(GL_TEXTURE_2D, 0);
//        GL_CHECK_ERRORS;
//
//        glBindVertexArray(VAO);
//
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
//        glEnableVertexAttribArray(0);
//
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
//        glEnableVertexAttribArray(1);
//
////        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//        glBindVertexArray(0);
//    }
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Model ourModel(std::string("../objects/aircraft/E 45 Aircraft_obj.obj"));
    GL_CHECK_ERRORS;

    // Skybox
    //
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    std::string dir = "../textures/skybox/lightblue/";
    std::vector<std::string> faces = {
            dir + "right.png",
            dir + "left.png",
            dir + "top.png",
            dir + "bot.png",
            dir + "front.png",
            dir + "back.png"
    };

    unsigned int cubemapTexture = loadCubemap(faces);

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

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, -5.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -2.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -3.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    GL_CHECK_ERRORS;

    //цикл обработки сообщений и отрисовки сцены каждый кадр
    while (!glfwWindowShouldClose(window)) {
        GL_CHECK_ERRORS
        glfwPollEvents();
        GL_CHECK_ERRORS

        //очищаем экран каждый кадр
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL_CHECK_ERRORS

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        move();

        glm::mat4 model(1.0);
        glm::mat4 view(1.0);
        glm::mat4 proj(1.0);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        proj = glm::perspective(glm::radians(fov), (GLfloat) WIDTH / HEIGHT, 0.1f, 100.0f);

        shader_skybox.StartUseShader();
        shader_skybox.SetUniform("view", view);
        shader_skybox.SetUniform("proj", proj);

        // skybox draw
        //
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
        shader_skybox.StopUseShader();

        shader_enemy.StartUseShader();
        model = glm::translate(model, glm::vec3(1.0, 1.0, -10.0));
        shader_enemy.SetUniform("model", model);
        shader_enemy.SetUniform("view", view);
        shader_enemy.SetUniform("proj", proj);

        ourModel.Draw(shader_enemy);

        shader_enemy.StopUseShader();

//
//        // draw call
//        //
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glBindVertexArray(VAO);
//
//        for (GLuint i = 0; i < 10; i++) {
//            model = glm::translate(model, cubePositions[i] +
//                                          glm::vec3(sin(glm::radians(currentFrame)), cos(glm::radians(currentFrame)),
//                                                    0.0f));
//            GLfloat angle = glm::radians(10.0f) * i;
//            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//            program.SetUniform("model", model);
//            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        }
//        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

//    //очищаем vbo и vao перед закрытием программы
//    //
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxEBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

int initGL() {
    int res = 0;
    //грузим функции opengl через glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key == GLFW_KEY_0) && (action == GLFW_PRESS)) {
        cameraPos = glm::vec3(0.0, 0.0, 3.0);
        cameraFront = glm::vec3(0.0, 0.0, -1.0);
        cameraUp = glm::vec3(0.0, 1.0, 0.0);
        firstMouse = true;
    }
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (fov >= 30.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 30.0f)
        fov = 30.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void move() {
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W]) cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S]) cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_SPACE]) cameraPos += cameraSpeed * cameraUp;
    if (keys[GLFW_KEY_C]) cameraPos -= cameraSpeed * cameraUp;
}

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
