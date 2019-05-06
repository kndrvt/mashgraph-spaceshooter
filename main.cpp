//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Skybox.h"
#include "Enemy.h"
#include "Asteroid.h"
#include "Bullet.h"


//External dependencies
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>

static const GLsizei WIDTH = 800, HEIGHT = 600; //размеры окна

Camera camera(WIDTH, HEIGHT);
bool keys[1024];
bool firstMouse = true;
bool shoot = false;
bool GodMode = false;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
GLfloat deltaTime = 0.0f;    // Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;    // Время вывода последнего кадра

int initGL();
GLfloat random_range(int end, int begin);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mousebuttonCallback(GLFWwindow *window, int button, int action, int mods);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void move();

int main(int argc, char **argv) {

    if (!glfwInit()) {
        return -1;
    }

    //запрашиваем контекст opengl версии 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Space shooter", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mousebuttonCallback);
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

    shaders[GL_VERTEX_SHADER] = "../shaders/skybox/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/skybox/fragment.glsl";
    ShaderProgram shader_skybox(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/enemy/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/enemy/fragment.glsl";
    ShaderProgram shader_enemy(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/asteroid/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/asteroid/fragment.glsl";
    ShaderProgram shader_asteroid(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/bullet/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/bullet/fragment.glsl";
    ShaderProgram shader_bullet(shaders);
    GL_CHECK_ERRORS

    glfwSwapInterval(1); // force 60 frames per second

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Skybox
    //
    Skybox skybox(std::string("../textures/skybox/lightblue/"));
    GL_CHECK_ERRORS

    // Enemy
    //
    Enemy enemy(std::string("../objects/aircraft/E 45 Aircraft_obj.obj"));
    GL_CHECK_ERRORS

    // Asteroid
    //
    Asteroid asteroid(std::string("../objects/planet/planet.obj"));
    GL_CHECK_ERRORS

    // Bullet
    //
    Bullet bullet(camera.Pos, glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f)));
    bullet.Color = glm::vec3(0.8, 0.0, 1.0);
    GL_CHECK_ERRORS

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vector<glm::vec3> Positions;
    for (int i = 0; i < 10; ++i) {
        Positions.push_back(glm::vec3(random_range(8, 5), 0.0, -5.0));
    }
    GL_CHECK_ERRORS

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

        view = camera.GetViewMatrix();
        proj = camera.GetPerspectiveMatrix();

        // skybox draw
        //
        shader_skybox.StartUseShader();
        shader_skybox.SetUniform("view", view);
        shader_skybox.SetUniform("proj", proj);
        skybox.Draw();
        shader_skybox.StopUseShader();

        // enemy draw
        //
        if (glm::length(enemy.Pos - bullet.Pos) < 3.0f) {
            enemy.dead = true;
        }
        if (enemy.Pos.z > 0.0f || enemy.dead) {
            glm::vec2 tmp(random_range(5, 0), random_range(5, 0));
            enemy.Pos = glm::vec3(tmp, -100.0f);
            enemy.Front = glm::normalize(glm::vec3(tmp, 1.0f));
            enemy.dead = false;
        }
        if (enemy.Pos.z > -5.0f) {
            enemy.Front = glm::vec3(0.0);
        }
        enemy.movement(deltaTime);
        if (((int) rand() % 10) == 0 && !enemy.dead) enemy.shoot();

        enemy.draw(shader_enemy, camera);

        // asteroid draw
        //
        shader_asteroid.StartUseShader();
        shader_asteroid.SetUniform("view", view);
        shader_asteroid.SetUniform("proj", proj);
        for (int i = 0; i < 10; ++i) {
            model = glm::mat4(1.0);
            model = glm::translate(model, Positions[i]);
            model = glm::rotate(model, -180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
            shader_asteroid.SetUniform("model", model);
            asteroid.Draw(shader_asteroid);
        }
        shader_asteroid.StopUseShader();

        // bullet draw
        //
        if (shoot) {
            bullet.update(camera.Pos, camera.Front);
            shoot = false;
        }
        bullet.movement(deltaTime);
        enemy.bullet.movement(deltaTime);

        bullet.draw(shader_bullet, camera);
        enemy.bullet.draw(shader_bullet, camera);

        glfwSwapBuffers(window);
    }

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

GLfloat random_range(int end, int begin) {
    srand(time(NULL));
    if ((int) rand() % 2) return (int) rand() % end + begin;
    else return -((int) rand() % end + begin);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key == GLFW_KEY_0) && (action == GLFW_PRESS)) {
        camera.begin();
        GodMode = false;
        firstMouse = true;
    }
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mousebuttonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) shoot = true;
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

    camera.ProcessMouseMovement(xoffset, yoffset, GodMode);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void move() {
    if (GodMode) {
        if (keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (keys[GLFW_KEY_SPACE])
            camera.ProcessKeyboard(UP, deltaTime);
        if (keys[GLFW_KEY_C])
            camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (keys[GLFW_KEY_9])
        GodMode = true;
}
