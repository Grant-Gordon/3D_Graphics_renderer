// system
#include <iostream>
#include <array>

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
    "/home/emergentstupidity/persProj/3D_Graphics_renderer/sandbox//model_loading/";
std::string MODEL_DIR = PROJECT_DIR + "Models/";
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

    //load shaders
    Shader lightCubeShaderProgram{(SHADERS_DIR + "lightBoxVertex.glsl").c_str(),
        (SHADERS_DIR + "lightBoxFragment.glsl").c_str()};
    Shader multiLightShaderProgram{(SHADERS_DIR + "multiLightVertex.glsl").c_str(),
        (SHADERS_DIR + "multiLightFragment.glsl").c_str()};
    //
    // Shader bareModelShaderProgram{(SHADERS_DIR + "bareModelVertex.glsl").c_str(),
    //     (SHADERS_DIR + "bareModelFragment.glsl").c_str()};
    //
    stbi_set_flip_vertically_on_load(true);


    //load Models
    Model backpackModel((MODEL_DIR + "backpack/backpack.obj").c_str());
    Model cubeModel ((MODEL_DIR + "cube/Cube.obj").c_str());

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3( -4.0f, 2.0f, -12.0f),
        glm::vec3( 0.0f, 0.0f, -3.0f),
    };
    glm::vec3 pointLightColors[] = {
        glm::vec3(1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    };

    // defined outside of loop as projection rarely changes
    glm::mat4 projectionTransform = glm::mat4(1.0f);
    projectionTransform =
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
        // bareModelShaderProgram.use();
        // glm::mat4 view_transform = camera.GetViewMatrix();
        // bareModelShaderProgram.setMat4("projectionTransform", projection_transform);
        // bareModelShaderProgram.setMat4("viewTransform", view_transform);

        // //render loaded model
        // glm::mat4 modelTransform = glm::mat4(1.0f);
        // modelTransform = glm::translate(modelTransform, glm::vec3(0.0f));
        // modelTransform = glm::scale(modelTransform, glm::vec3(1.0f));
        // bareModelShaderProgram.setMat4("modelTransform", model_transform);
        // backpackModel.Draw(bareModelShaderProgram);
        
        //set lighting values

        //render light cubes
        glm::mat4 viewTransform = camera.GetViewMatrix();
        for(size_t i{0}; i < pointLightPositions.size(); ++i){
            glm::mat4 modelTransform = glm::mat4(1.0f);
            modelTransform = glm::translate(modelTransform, pointLightPositions[i]);
            lightCubeShaderProgram.setVec3("lightColor", pointLightColors[i]);

            lightCubeShaderProgram.setMat4("model_transform", modelTransform);
            lightCubeShaderProgram.setMat4("projection_transform", projectionTransform);
            lightCubeShaderProgram.setMat4("view_transform", viewTransform);

            cubeModel.Draw(lightCubeShaderProgram);
        }

        // set transforms
        multiLightShaderProgram.use();
        viewTransform = camera.GetViewMatrix();
        multiLightShaderProgram.setMat4("projectionTransform", projectionTransform);
        multiLightShaderProgram.setMat4("viewTransform", viewTransform);
        //render backpack model
        glm::mat4 modelTransform = glm::mat4(1.0f);
        modelTransform = glm::translate(modelTransform, glm::vec3(0.0f));
        modelTransform = glm::scale(modelTransform, glm::vec3(1.0f));
        multiLightShaderProgram.setMat4("modelTransform", modelTransform);
        backpackModel.Draw(multiLightShaderProgram);
        


        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(SDL_window);
        endFrameTicks = SDL_GetTicks();
        deltaTime = (endFrameTicks - startFrameTicks) / 1000.0f;
    }

    //===========================================
    // Deconstructors/ End Game Loop
    //===========================================
    SDL_DestroyWindow(SDL_window);
    return 0;
}
