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
#include "camera.h"
#include "stb_image.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;
std::string PROJECT_DIR = "/home/emergentstupidity/persProj/3D_Graphics_renderer/sandbox/lighting/colors/";
std::string TEXTURE_DIR = PROJECT_DIR + "Textures/";
std::string TEXTURE_FILE = "container.jpg";
std::string TEXTURE_PATH = TEXTURE_DIR + TEXTURE_FILE;
std::string SHADERS_DIR = PROJECT_DIR + "src/shaders/";
std::string VERTEX_SHADER_PATH = SHADERS_DIR + "woodenBoxVertex.glsl";
std::string FRAGMENT_SHADER_PATH = SHADERS_DIR + "woodenBoxFragment.glsl";
std::string LIGHT_FRAGMENT_SHADER_PATH = SHADERS_DIR + "lightBoxFragment.glsl";
std::string LIGHT_VERTEX_SHADER_PATH = SHADERS_DIR + "lightBoxVertex.glsl";

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

    // Vertex shader out, and fragment shader in must have same name
    Shader woodenBoxShaderProgram{VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str()};
    Shader lightShaderProgram{LIGHT_VERTEX_SHADER_PATH.c_str(), LIGHT_FRAGMENT_SHADER_PATH.c_str()};

    // clang-format off
    // pos(xyz), normal unit vector(xyz), texture(uv). 6 vertices per face, 6 faces 
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,   
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,    
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,    
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,   
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  
                                                                   
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f,  0.0f,
                                                                   
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                                                   
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                                                                   
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                                                                  
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
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

    //========================
    // VBO(data) and VAO (attrib interpretation)
    //========================
    // VBO: RAW vertex data
    // -------------------------
    GLuint VBO;
    glGenBuffers(1, &VBO);              // buffers are what is being batched CPU->GPU for reduced IO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds VBO to GL_ARRAY_BUFFER target (i.e sets VBO as "the one being used)
    // allocates size and puts vertices[] data onto GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // VAO: how VBO should be interpreted.
    // ----------------------------------------
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // Make VAO the current VAO by binding it

    // Hardcode attribute location since VAO defines attrib location.
    // Position attribs
    // Defines interpretation of attrib from bound VBO onto bound VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    // Normal unit vector pos
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0); // Enable pos attrib
    glEnableVertexAttribArray(1); // Enable normal unit vector attrib
    glEnableVertexAttribArray(2); // Enable tex attrib


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

    woodenBoxShaderProgram.use();
    woodenBoxShaderProgram.setInt("samplerTexture", 0);

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

        // bind texture
        glBindTexture(GL_TEXTURE_2D, texture);
        // if using multiple textures, glActivateTexture(GLTEXTURE<1-16>) the bind,
        // for each texture

        // bind VAO so gl knows to use it
        glBindVertexArray(VAO);


        woodenBoxShaderProgram.use();
        // set phong material lighting values (just avoiding magic numbers)
        glm::vec3 ambientMaterialLevel = glm::vec3(1.0f, 0.5f, 0.41);
        glm::vec3 diffuseMaterialLevel = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 specularMaterialLevel = glm::vec3(0.5, 0.5f, 0.5f);
        float shininessMaterialLevel = 16.0f;
        woodenBoxShaderProgram.setVec3("material.ambientLevel", ambientMaterialLevel);
        woodenBoxShaderProgram.setVec3("material.diffuseLevel", diffuseMaterialLevel);
        woodenBoxShaderProgram.setVec3("material.specularLevel", specularMaterialLevel);
        woodenBoxShaderProgram.setFloat("material.shininessLevel", shininessMaterialLevel);

        glm::vec3 lightColor;
        lightColor.x = sin((float)(SDL_GetTicks64() / 1000.0) * 2.0f);
        lightColor.y = sin((float)(SDL_GetTicks64() / 1000.0) * 0.7f);
        lightColor.z = sin((float)(SDL_GetTicks64() / 1000.0) * 1.3f);

        glm::vec3 diffuseLightLevel = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientLightLevel = diffuseLightLevel * glm::vec3(0.2f);
        glm::vec3 specularLightLevel = glm::vec3(1.0f, 1.0f, 1.0f);
        woodenBoxShaderProgram.setVec3("light.ambientLevel", ambientLightLevel);
        woodenBoxShaderProgram.setVec3("light.diffuseLevel", diffuseLightLevel);
        woodenBoxShaderProgram.setVec3("light.specularLevel", specularLightLevel);

        glm::vec3 lightBoxPos = glm::vec3(5.0, 5.0, -5.0f);
        // glm::vec3 objectColor(0.2, 0.7, 0.7);
        // woodenBoxShaderProgram.setVec3("material.color", objectColor);
        woodenBoxShaderProgram.setVec3("light.color", lightColor);
        woodenBoxShaderProgram.setVec3("light.position", lightBoxPos);
        woodenBoxShaderProgram.setVec3("viewPos", camera.Position);


        // create tansformations - instantiate matrices. Reset for each cube
        // otherwise transform matrices accumulate;
        glm::mat4 model_transform;
        glm::mat4 view_transform = camera.GetViewMatrix();
        // Often good pracrice to set outside of the main loop as projection
        //      matrix rarely changes
        woodenBoxShaderProgram.setMat4("projection_transform", projection_transform);
        // // draw normal objects (not light box)
        for(GLuint i = 0; i < 10; i++) {
            model_transform = glm::mat4(1.0f);
            model_transform = glm::translate(model_transform, cubePositions[i]);
            model_transform =
                glm::rotate(model_transform, (float)(SDL_GetTicks64() / 1000.0 + i * 2), glm::vec3(1.0f, 1.0f, 1.0f));
            woodenBoxShaderProgram.setMat4("model_transform", model_transform);
            woodenBoxShaderProgram.setMat4("view_transform", view_transform);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // draw lightbox
        lightShaderProgram.use();

        model_transform = glm::mat4(1.0f);
        model_transform = glm::translate(model_transform, lightBoxPos);
        model_transform = glm::scale(model_transform, glm::vec3(4.0f));

        lightShaderProgram.setMat4("projection_transform", projection_transform);
        lightShaderProgram.setMat4("view_transform", view_transform);
        lightShaderProgram.setMat4("model_transform", model_transform);
        lightShaderProgram.setVec3("lightColor", lightColor);

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
