// system
#include <iostream>

// Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mouse.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <chrono>
#include "Shader.h"
#include "camera.h"
#include "stb_image.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;
std::string PROJECT_DIR = "/home/emergentstupidity/projects/3D_Graphics_renderer/sandbox/lighting/colors/";
std::string TEXTURE_DIR = PROJECT_DIR + "Textures/";
std::string TEXTURE_FILE = "container.jpg";
std::string TEXTURE_PATH = TEXTURE_DIR + TEXTURE_FILE;
std::string SHADERS_DIR = PROJECT_DIR + "src/shaders/";
std::string VERTEX_SHADER_PATH = SHADERS_DIR + "wooden_box.vs";
std::string FRAGMENT_SHADER_PATH = SHADERS_DIR + "wooden_box.fs";
std::string LIGHT_FRAGMENT_SHADER_PATH = SHADERS_DIR + "lightBox.fs";
std::string LIGHT_VERTEX_SHADER_PATH = SHADERS_DIR + "lightBox.vs";


int main() {
    //===========================================
    // SDL init
    //===========================================
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    // TODO: glViewport(GLuint x Gluint y, width, height); // For normalizing I
    // beleive?
    SDL_Window* SDL_window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
        SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(SDL_window == NULL) {
        printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetRelativeMouseMode(SDL_TRUE); // for fps so mouse doesn't stop at windows edge

    // create GL Context
    SDL_GLContext glContext = SDL_GL_CreateContext(SDL_window);
    if(glContext == NULL) {
        printf("OpenGL context could not be created. SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    // glad: laod all OpenGL function pointers
    if(!gladLoadGLLoader((SDL_GL_GetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Vertex shader out, and fragment shader in must have same name
    Shader shaderProgram{VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str()};
    Shader lightShaderProgram{LIGHT_VERTEX_SHADER_PATH.c_str(), LIGHT_FRAGMENT_SHADER_PATH.c_str()};

    // vertex pos
    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f

    };

    // for multiple cubes:

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f), 
        glm::vec3(1.3f, -2.0f, -2.5f), 
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    // clang-format on
    // VAO stores one or more VBO pointers. Tells GL how to interpret them
    // first fo cube vao/vbo
    GLuint VAO, VBO;

    // VAO createion must come before VBO
    glGenVertexArrays(1, &VAO);
    // buffers are what is being batched CPU->GPU for reduced IO
    glGenBuffers(1, &VBO);
    // copy vertex data into buffers memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Make VAO the current VAO by binding it
    glBindVertexArray(VAO);

    // Get shader attrib location, and enable it.
    GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "vInPos");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
        (void*)0); // defines array of vertex attrib data
    glEnableVertexAttribArray(posAttrib);


    // now config lightBox vao, (VBO stays same; vertices are teh same for light objext, also a 3d objext).
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
        (void*)0); // defines array of vertex attrib data
    glEnableVertexAttribArray(posAttrib);

    // texture coord attribute
    GLuint texAttrib = glGetAttribLocation(shaderProgram.ID, "vInTexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    //========================
    // load and generate texture
    //========================

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture
    // object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(TEXTURE_PATH.c_str(), &width, &height, &nrChannels, 0);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shaderProgram.use();
    shaderProgram.setInt("samplerTexture", 0);

    // defined outside of loop as projection rarely changes
    glm::mat4 projection_transform = glm::mat4(1.0f);
    projection_transform =
        glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 100.0f);

    //===========================================
    // Game Loop
    //===========================================
    bool windowShouldClose = false;
    SDL_Event event;

    // camera init
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    Camera camera{cameraPos};
    Camera::CameraMovement cameraDirection;

    double deltaTime = 0.0f;
    int endFrameTicks = SDL_GetTicks();
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    while(!windowShouldClose) {
        int startFrameTicks = SDL_GetTicks();
        // user input
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    windowShouldClose = true;
                    break;
                case SDL_MOUSEMOTION:
                    xOffset = static_cast<float>(event.motion.xrel);
                    yOffset = static_cast<float>(-event.motion.yrel);
                    camera.ProcessMouseMovement(xOffset, yOffset);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_h:
                        case SDLK_a:
                        case SDLK_LEFT:
                            // left
                            cameraDirection = Camera::CameraMovement::LEFT;
                            break;
                        case SDLK_j:
                        case SDLK_s:
                        case SDLK_DOWN:
                            // backward
                            cameraDirection = Camera::CameraMovement::BACKWARD;
                            break;
                        case SDLK_k:
                        case SDLK_w:
                        case SDLK_UP:
                            // forward
                            cameraDirection = Camera::CameraMovement::FORWARD;
                            break;
                        case SDLK_l:
                        case SDLK_d:
                        case SDLK_RIGHT:
                            // right-
                            cameraDirection = Camera::CameraMovement::RIGHT;
                            break;
                        case SDLK_q:
                            if(SDL_GetModState() & KMOD_SHIFT) {
                                windowShouldClose = true;
                            }
                            break;
                    }
                    camera.ProcessKeyboard(cameraDirection, deltaTime);
                    break;
            }
        }
        // sets what the background color will clear to when glClear() is called
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // if using multiple textures, glActivateTexture(GLTEXTURE<1-16>) the bind,
        // for each texture

        // bind VAO so gl knows to use it
        glBindVertexArray(VAO);

        shaderProgram.use();
        shaderProgram.setVec3("objectColor", 1.0f, 0.5f, 0.2f);
        shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);


        // create tansformations - instantiate matrices. Reset for each cube
        // otherwise transform matrices accumulate;
        glm::mat4 model_transform;
        glm::mat4 view_transform;
        // draw normal objects (not light box)
        for(GLuint i = 0; i < 10; i++) {
            model_transform = glm::mat4(1.0f);
            view_transform = glm::mat4(1.0f);
            model_transform = glm::translate(model_transform, cubePositions[i]);
            model_transform =
                glm::rotate(model_transform, (float)(SDL_GetTicks64() / 1000.0 + i * 2), glm::vec3(1.0f, 1.0f, 1.0f));
            view_transform = camera.GetViewMatrix();
            shaderProgram.setMat4("model_transform", model_transform);
            shaderProgram.setMat4("view_transform", view_transform);
            // Often good pracrice to set outside of the main loop as projection
            // matrix rarely changes
            shaderProgram.setMat4("projection_transform", projection_transform);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // draw lightbox
        glm::vec3 lightBoxPos = glm::vec3(2.0); // cubePositions[1];
        lightShaderProgram.use();
        lightShaderProgram.setMat4("model_transform", model_transform);
        lightShaderProgram.setMat4("view_transform", view_transform);
        model_transform = glm::mat4(1.0f);
        model_transform = glm::translate(model_transform, lightBoxPos);
        model_transform = glm::scale(model_transform, glm::vec3(4.0f));
        shaderProgram.setMat4("model_transform", model_transform);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(SDL_window);
        endFrameTicks = SDL_GetTicks();
        deltaTime = (endFrameTicks - startFrameTicks) / 1000.0f;
    }

    //===========================================
    // Deconstructors/ End Game Loop
    //===========================================
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    SDL_DestroyWindow(SDL_window);
    return 0;
}
