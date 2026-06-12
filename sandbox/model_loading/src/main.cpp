// system
#include <iostream>

// Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <chrono>
#include "Shader.h"
#include "Model.h"
#include "camera.h"
#include "stb_image.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;
std::string PROJECT_DIR =
    "/home/emergentstupidity/persProj/3D_Graphics_renderer/sandbox/lighting/model_loading/";
std::string MODEL_DIR = PROJECT_DIR + "Models/"
std::string TEXTURE_DIR = PROJECT_DIR + "Textures/";
std::string SHADERS_DIR = PROJECT_DIR + "src/shaders/";

void processUserInput(bool& windowShouldClose, Camera& camera, double& deltaTime) {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            // closing conditions
            case SDL_QUIT:
                windowShouldClose = true;
                break;
            case SDL_KEYDOWN: // maybe just put in sdlgetkeyboard state? not sure on which is more performant
                if(event.key.keysym.sym == SDLK_q) {
                    if(SDL_GetModState() & KMOD_SHIFT) {
                        windowShouldClose = true;
                    }
                }
                break;
            // mouse movement
            case SDL_MOUSEMOTION: { // need local scope because defining var inside case
                camera.ProcessMouseMovement(static_cast<float>(event.motion.xrel),
                    static_cast<float>(-event.motion.yrel));
                break;
            }
        }
    }
    // Keyboard input
    //  sdlGetKeyboardState for easier "is key held?" logic
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    if(keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_K] || keyState[SDL_SCANCODE_UP]) {
        camera.ProcessKeyboard(Camera::CameraMovement::FORWARD, deltaTime);
    }
    if(keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_L] || keyState[SDL_SCANCODE_LEFT]) {
        camera.ProcessKeyboard(Camera::CameraMovement::LEFT, deltaTime);
    }
    if(keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_J] || keyState[SDL_SCANCODE_DOWN]) {
        camera.ProcessKeyboard(Camera::CameraMovement::BACKWARD, deltaTime);
    }
    if(keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_H] || keyState[SDL_SCANCODE_RIGHT]) {
        camera.ProcessKeyboard(Camera::CameraMovement::RIGHT, deltaTime);
    }
    if(keyState[SDL_SCANCODE_SPACE]) {
        camera.ProcessKeyboard(Camera::CameraMovement::UP, deltaTime);
    }
    if(keyState[SDL_SCANCODE_LSHIFT] || keyState[SDL_SCANCODE_RSHIFT]) {
        camera.ProcessKeyboard(Camera::CameraMovement::DOWN, deltaTime);
    }
}


unsigned int loadTexture(char const* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    GLint width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if(!data) {
        std::cout << "Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
        return textureID;
    }
    // specify data format
    GLenum format;
    if(nrComponents == 1) {
        format = GL_RED;
    } else if(nrComponents == 3) {
        format = GL_RGB;
    } else if(nrComponents == 4) {
        format = GL_RGBA;
    } else {
        std::cout << "value of nrComponents (" << nrComponents << ") is not supported for textures" << std::endl;
        stbi_image_free(data);
        return textureID;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping/filtering options (on the currently bound texture
    // object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return textureID;
}

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


    Shader lightCubeShaderProgram{(SHADERS_DIR + "lightBoxVertex.glsl").c_str(),
        (SHADERS_DIR + "lightBoxFragment.glsl").c_str()};
    Shader multiLightShaderProgram{(SHADERS_DIR + "multiLightVertex.glsl").c_str(),
        (SHADERS_DIR + "multiLightFragment.glsl").c_str()};
    
    stbi_set_flip_vertically_on_load(true);
    Model backpackModel((MODELS_DIR + "backpack/backpack.obj").c_str());


    // defined outside of loop as projection rarely changes
    glm::mat4 projection_transform = glm::mat4(1.0f);
    projection_transform =
        glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 100.0f);

    //===========================================
    // Game Loop
    //===========================================
    bool windowShouldClose = false;

    // camera init
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    Camera camera{cameraPos};

    double deltaTime = 0.0f;
    int endFrameTicks = SDL_GetTicks();
    while(!windowShouldClose) {
        int startFrameTicks = SDL_GetTicks();

        // user input
        processUserInput(windowShouldClose, camera, deltaTime);

        // sets what the background color will clear to when glClear() is called
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //set projection and view transforms
        multiLightShaderProgram.use();
        glm::mat4 view_transform = camera.GetViewMatrix();
        multiLightShaderProgram.setMat4("projection_transform", projection_transform);
        multiLightShaderProgram.setMat4("view_transform", view_transform);
        
        //render loaded model
        glm::mat4 model_transform = glm::mat4(1.0f);
        model_transform = glm::translate(model_transform, glm::vec3(0.0f));
        model_transform = glm::scale(model_transform, glm::vec3(1.0f));
        multiLightShaderProgram.setMat4("model_transform", model_transform);
        backpackModel.Draw(multiLightShaderProgram);


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
