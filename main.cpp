//internal includes
#include "common.h"
#include "ShaderProgram.h"
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

bool keys[1024];
bool firstMouse = true;
bool shoot = false;
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
GLfloat random_range(int end, int begin);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mousebuttonCallback(GLFWwindow* window, int button, int action, int mods);
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
    Bullet bullet(std::string("../objects/planet/planet.obj"));
    GL_CHECK_ERRORS

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 enemy_pos(0.0f, 0.0f, -100.0f);
    glm::vec3 bullet_pos = cameraPos;
    glm::vec3 bullet_target = cameraFront;
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

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        proj = glm::perspective(glm::radians(fov), (GLfloat) WIDTH / HEIGHT, 0.1f, 100.0f);

        // skybox draw
        //
        shader_skybox.StartUseShader();
        shader_skybox.SetUniform("view", view);
        shader_skybox.SetUniform("proj", proj);
        skybox.Draw();
        shader_skybox.StopUseShader();

        // enemy draw
        //
        if (enemy_pos.z > 0.0f) {
            enemy_pos.x = random_range(5, 0);
            enemy_pos.y = random_range(5, 0);
            enemy_pos.z = -100.0f;
        }
        GLfloat enemySpeed = 30.0f * deltaTime;
        enemy_pos.z += enemySpeed;

        shader_enemy.StartUseShader();
        model = glm::mat4(1.0);
        model = glm::translate(model, enemy_pos);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

        shader_enemy.SetUniform("model", model);
        shader_enemy.SetUniform("view", view);
        shader_enemy.SetUniform("proj", proj);
        enemy.Draw(shader_enemy);
        shader_enemy.StopUseShader();

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
            bullet_target = cameraFront;
            bullet_pos = cameraPos;
            shoot = false;
        }
        GLfloat bulletSpeed = 10.0f * deltaTime;
        bullet_pos += bulletSpeed * bullet_target;

        shader_bullet.StartUseShader();
        model = glm::mat4(1.0);
        model = glm::translate(model, bullet_pos);
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
        shader_bullet.SetUniform("model", model);
        shader_bullet.SetUniform("view", view);
        shader_bullet.SetUniform("proj", proj);
        bullet.Draw(shader_bullet);
        shader_asteroid.StopUseShader();

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
    if ((int)rand() % 2) return (int)rand() % end + begin;
    else return -((int)rand() % end + begin);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key == GLFW_KEY_0) && (action == GLFW_PRESS)) {
        cameraPos = glm::vec3(0.0, 0.0, 3.0);
        cameraFront = glm::vec3(0.0, 0.0, -1.0);
        cameraUp = glm::vec3(0.0, 1.0, 0.0);
        firstMouse = true;
        lastX = WIDTH / 2;
        lastY = HEIGHT / 2;
        yaw = -90.0f;
        pitch = 0.0f;
        fov = 45.0f;
    }
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mousebuttonCallback(GLFWwindow* window, int button, int action, int mods) {
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
