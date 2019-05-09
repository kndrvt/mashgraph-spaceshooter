//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Skybox.h"
#include "Gamer.h"
#include "Enemy.h"
#include "Asteroid.h"

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
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mousebuttonCallback(GLFWwindow *window, int button, int action, int mods);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void move();

int main(int argc, char **argv) {

    srand(time(NULL));
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

    shaders[GL_VERTEX_SHADER] = "../shaders/gamer/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/gamer/fragment.glsl";
    ShaderProgram shader_gamer(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/bullet/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/bullet/fragment.glsl";
    ShaderProgram shader_bullet(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/asteroid/vertex.glsl";
    shaders[GL_GEOMETRY_SHADER] = "../shaders/asteroid/geometry.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/asteroid/fragment.glsl";
    ShaderProgram shader_asteroid(shaders);
    GL_CHECK_ERRORS

    shaders[GL_VERTEX_SHADER] = "../shaders/enemy/vertex.glsl";
    shaders[GL_GEOMETRY_SHADER] = "../shaders/enemy/geometry.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/enemy/fragment.glsl";
    ShaderProgram shader_enemy(shaders);
    GL_CHECK_ERRORS

    glfwSwapInterval(1); // force 60 frames per second
    {
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Skybox
        //
        Skybox skybox(std::string("../textures/skybox/lightblue/"));
        GL_CHECK_ERRORS

        // Gamer
        //
        Gamer gamer(camera);
        GL_CHECK_ERRORS

        // Enemy
        //
        vector<Enemy> enemies = {
                Enemy(std::string("../objects/aircraft/E 45 Aircraft_obj.obj")),
                Enemy(std::string("../objects/aircraft/E 45 Aircraft_obj.obj")),
                Enemy(std::string("../objects/aircraft/E 45 Aircraft_obj.obj")),
                Enemy(std::string("../objects/spaceship2/City Patrol Vehicle/City Patrol Vehicle.obj"), 0.5, 180.0),
                Enemy(std::string("../objects/spaceship2/City Patrol Vehicle/City Patrol Vehicle.obj"), 0.5, 180.0),
                Enemy(std::string("../objects/spaceship1/Intergalactic_Spaceship-(Wavefront).obj")),
        };
        GL_CHECK_ERRORS

        // Asteroid
        //
        vector<Asteroid> asteroids = {
                Asteroid(std::string("../objects/asteroid2/OBJ.obj")),
                Asteroid(std::string("../objects/asteroid2/OBJ.obj")),
                Asteroid(std::string("../objects/asteroid2/OBJ.obj")),
                Asteroid(std::string("../objects/asteroid2/OBJ.obj")),
                Asteroid(std::string("../objects/asteroid3/asteroidOBJ.obj")),
                Asteroid(std::string("../objects/asteroid3/asteroidOBJ.obj")),
                Asteroid(std::string("../objects/asteroid3/asteroidOBJ.obj")),
                Asteroid(std::string("../objects/asteroid3/asteroidOBJ.obj"))
        };
        GL_CHECK_ERRORS

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

            // skybox draw
            //
            skybox.draw(shader_skybox, camera, currentFrame);

            // enemy draw
            //
            for (int i = 0; i < enemies.size(); ++i) {
                if (!enemies[i].dead) {
                    for (int j = 0; j < asteroids.size(); ++j) {
                        if (asteroids[j].destroyed) continue;
                        if (glm::length(asteroids[j].Pos - enemies[i].Pos) <=
                            (asteroids[j].Radius + enemies[i].Radius)) {
                            enemies[i].death(currentFrame);
                            asteroids[j].destruction(currentFrame);
                        }
                    }
                    if (glm::length(enemies[i].Pos - camera.Pos) <= gamer.Radius) {
                        if (!GodMode) gamer.damage();
                    }

                    if ((int) round(currentFrame) % 5 == 0) enemies[i].shoot(camera);
                }
                enemies[i].movement(deltaTime);
                enemies[i].draw(shader_enemy, shader_bullet, camera, deltaTime, currentFrame);
            }

            // asteroid draw
            //
            for (int j = 0; j < asteroids.size(); ++j) {
                asteroids[j].movement(deltaTime);
                asteroids[j].draw(shader_asteroid, camera, currentFrame);
            }

            // gamer draw
            //
            if (shoot) {
                shoot = false;
                gamer.shoot(camera);
            }
            for (int i = 0; i < enemies.size(); ++i) {
                if (enemies[i].dead) continue;
                if (glm::length(enemies[i].Pos - camera.Pos) <= enemies[i].Radius + gamer.Radius) {
                    if (!GodMode) gamer.damage();
                    enemies[i].death(currentFrame);
                }
                if (glm::length(enemies[i].Pos - gamer.bullet.Pos) <= enemies[i].Radius) {
                    gamer.hit(camera);
                    enemies[i].damage(currentFrame);
                }
            }
            for (int j = 0; j < asteroids.size(); ++j) {
                if (asteroids[j].destroyed) continue;
                if (glm::length(asteroids[j].Pos - camera.Pos) <= asteroids[j].Radius + gamer.Radius) {
                    gamer.damage();
                    asteroids[j].destruction(currentFrame);
                }
                if (glm::length(asteroids[j].Pos - gamer.bullet.Pos) <= asteroids[j].Radius) {
                    gamer.hit(camera);
                    asteroids[j].destruction(currentFrame);
                }
            }
            gamer.draw(shader_gamer, shader_bullet, camera, deltaTime);

            glfwSwapBuffers(window);
            if (gamer.EndGame) break;
        }
    }
    glfwTerminate();
    return 0;
}

int initGL() {
    int res = 0;

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
        camera.begin();
        GodMode = false;
        firstMouse = true;
    }
    if ((key == GLFW_KEY_9) && (action == GLFW_PRESS)) {
        GodMode = true;
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

}
