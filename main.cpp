//internal includes
#include "common.h"
#include "ShaderProgram.h"

//External dependencies
#include <GLFW/glfw3.h>
#include "SOIL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

static const GLsizei WIDTH = 640, HEIGHT = 480; //размеры окна

bool keys[1024];
bool firstMouse = true;
GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat fov = 45.0f;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
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

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 30.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 30.0f)
        fov = 30.0f;
    if(fov >= 45.0f)
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

//unsigned int loadCubemap(vector<std::string> faces)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//    int width, height, nrChannels;
//    for (unsigned int i = 0; i < faces.size(); i++)
//    {
//        unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
//            );
//            SOIL_free_image_data(data);
//        }
//        else
//        {
//            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//            SOIL_free_image_data(data);
//        }
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//    return textureID;
//}

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

    //создание шейдерной программы из двух файлов с исходниками шейдеров
    //используется класс-обертка ShaderProgram
    std::unordered_map<GLenum, std::string> shaders;
    shaders[GL_VERTEX_SHADER] = "../shaders/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment.glsl";
    ShaderProgram program(shaders);

    glfwSwapInterval(1); // force 60 frames per second

    //Создаем и загружаем геометрию поверхности
    //
    GLuint VBO;
    GLuint EBO;
    GLuint VAO;
    GLuint texture;
    {
        GLfloat vertices[] = {
                0.5f,  0.5f, 0.5f,  1.0f, 1.0f,   // Верхний правый
                0.5f, -0.5f, 0.5f,  1.0f, 0.0f,   // Нижний правый
                -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,   // Нижний левый
                -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,    // Верхний левый
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   // Верхний правый
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   // Нижний правый
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   // Нижний левый
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f    // Верхний левый
        };
        GLuint indices[] = {
                0, 1, 2, // Передняя грань
                2, 3, 0,
                4, 5, 6, //Задняя грань
                6, 7, 4,
                4, 5, 1, // Правая боковая грань
                1, 0, 4,
                7, 6, 2, // Левая боковая грань
                2, 3, 7,
                6, 5, 1, //Верхняя грань
                1, 2, 6,
                7, 4, 0,
                0, 3, 7  //Нижняя грань
        };

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height;
        unsigned char *image = SOIL_load_image("/media/kondrat/DATA/Programs/Graphics/task2/Sample/textures/space1.jpg",
                                               &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);




        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(0);
    }

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  2.0f, -5.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -2.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -3.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    //цикл обработки сообщений и отрисовки сцены каждый кадр
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        //очищаем экран каждый кадр
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.StartUseShader();

        // очистка и заполнение экрана цветом
        //
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw call
        //
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        move();

        glm::mat4 model(1.0);
        glm::mat4 view(1.0);
        glm::mat4 proj(1.0);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        proj = glm::perspective(glm::radians(fov), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);

        program.SetUniform("view", view);
        program.SetUniform("proj", proj);

        for(GLuint i = 0; i < 10; i++) {
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = glm::radians(10.0f) * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            program.SetUniform("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        program.StopUseShader();

        glfwSwapBuffers(window);
    }

    //очищаем vbo и vao перед закрытием программы
    //
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
