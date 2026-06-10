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
std::string PROJECT_DIR =
    "/home/emergentstupidity/persProj/3D_Graphics_renderer/sandbox/lighting/casters_and_multi_lights/";
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

    Shader lightShaderProgram{(SHADERS_DIR + "lightBoxVertex.glsl").c_str(),
        (SHADERS_DIR + "lightBoxFragment.glsl").c_str()};
    Shader woodenBoxShaderProgram{(SHADERS_DIR + "woodenBoxVertex.glsl").c_str(),
        (SHADERS_DIR + "woodenBoxFragment.glsl").c_str()};
    Shader steelRimmedCrateShaderProgram{(SHADERS_DIR + "steelRimmedCrateVertex.glsl").c_str(),
        (SHADERS_DIR + "steelRimmedCrateFragment.glsl").c_str()};
    Shader untexturedCubeShaderProgram{(SHADERS_DIR + "woodenBoxVertex.glsl").c_str(),
        (SHADERS_DIR + "untexturedCubeFragment.glsl").c_str()};

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


    GLuint woodenBoxDiffuseMap = loadTexture((TEXTURE_DIR + "woodenBoxDiffuseMap.jpg").c_str());
    GLuint steelRimmedCrateDiffuseMap = loadTexture((TEXTURE_DIR + "steelRimmedCrateDiffuseMap.png").c_str());
    GLuint steelRimmedCrateSpecularMap = loadTexture((TEXTURE_DIR + "steelRimmedCrateSpecularMap.png").c_str());

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

        // Bind Texture Object to Texture Unit Array slots
        glActiveTexture(GL_TEXTURE0); // where is is between 0 and GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS. Selects the slot
                                      // of textures looking at (in texture unit array)
        glBindTexture(GL_TEXTURE_2D,
            woodenBoxDiffuseMap); // assigns the texture to active slot (in texture unit array). GL_TEXTURE_2D =
                                  // location or target in GL context, changes how GL uses bound data

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, steelRimmedCrateDiffuseMap);

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, steelRimmedCrateSpecularMap);


        // bind VAO so gl knows to use it
        glBindVertexArray(VAO);


        // lightbox properties
        // if positional Light vec4.w will == 1.0, if its a direcitonal light, vec4.w will ==0.0
        glm::vec3 lightBoxPos = glm::vec3(5.0, 5.0, -5.0f);
        glm::vec3 sunDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
        // glm::vec3 lightColor;
        // lightColor.x = sin((float)(SDL_GetTicks64() / 1000.0) * 2.0f);
        // lightColor.y = sin((float)(SDL_GetTicks64() / 1000.0) * 0.7f);
        // lightColor.z = sin((float)(SDL_GetTicks64() / 1000.0) * 1.3f);

        glm::vec3 lightColor = glm::vec3(0.1, 0.2, 0.2);

        // woodenBox
        woodenBoxShaderProgram.use();
        // set phong material lighting values (just avoiding magic numbers)
        glm::vec3 ambientMaterialLevel = glm::vec3(1.0f, 0.5f, 0.41);
        glm::vec3 specularMaterialLevel = glm::vec3(0.5, 0.5f, 0.5f);
        float shininessMaterialLevel = 16.0f;
        woodenBoxShaderProgram.setVec3("material.ambientLevel", ambientMaterialLevel);
        woodenBoxShaderProgram.setVec3("material.specularLevel", specularMaterialLevel);
        woodenBoxShaderProgram.setInt("material.diffuseMap", 0);
        woodenBoxShaderProgram.setFloat("material.shininessLevel", shininessMaterialLevel);
        woodenBoxShaderProgram.setVec3("light.color", lightColor);
        woodenBoxShaderProgram.setVec3("light.direction", sunDirection);
        woodenBoxShaderProgram.setVec3("viewPos", camera.Position);

        glm::vec3 diffuseLightLevel = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientLightLevel = diffuseLightLevel * glm::vec3(0.2f);
        glm::vec3 specularLightLevel = glm::vec3(1.0f, 1.0f, 1.0f);
        woodenBoxShaderProgram.setVec3("light.ambientLevel", ambientLightLevel);
        woodenBoxShaderProgram.setVec3("light.diffuseLevel", diffuseLightLevel);
        woodenBoxShaderProgram.setVec3("light.specularLevel", specularLightLevel);


        // steelRimmedCrate
        steelRimmedCrateShaderProgram.use();
        steelRimmedCrateShaderProgram.setVec3("material.ambientLevel", ambientMaterialLevel);
        steelRimmedCrateShaderProgram.setFloat("material.shininessLevel", shininessMaterialLevel);
        steelRimmedCrateShaderProgram.setInt("material.diffuseMap", 1);
        steelRimmedCrateShaderProgram.setInt("material.specularMap", 2);
        //point light attribs
        steelRimmedCrateShaderProgram.setVec3("pointLight.ambientLevel", ambientLightLevel);
        steelRimmedCrateShaderProgram.setVec3("pointLight.diffuseLevel", diffuseLightLevel);
        steelRimmedCrateShaderProgram.setVec3("pointLight.specularLevel", specularLightLevel);
        steelRimmedCrateShaderProgram.setVec3("pointLight.color", lightColor);
            //attenuation
        steelRimmedCrateShaderProgram.setFloat("pointLight.constant", 1.0f);
        steelRimmedCrateShaderProgram.setFloat("pointLight.linear", 0.09f);
        steelRimmedCrateShaderProgram.setFloat("pointLight.quadratic", 0.032f);

        //flashlight
        glm::vec3 flashlightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 flashlightDiffuseLightLevel = lightColor * glm::vec3(0.5f);
        glm::vec3 flashlightAmbientLightLevel = flashlightDiffuseLightLevel * glm::vec3(0.2f);
        glm::vec3 flashlightSpecularLightLevel = glm::vec3(1.0f, 1.0f, 1.0f);
        steelRimmedCrateShaderProgram.setVec3("flashlight.ambientLevel", flashlightAmbientLightLevel);
        steelRimmedCrateShaderProgram.setVec3("flashlight.diffuseLevel", flashlightDiffuseLightLevel);
        steelRimmedCrateShaderProgram.setVec3("flashlight.specularLevel", flashlightSpecularLightLevel);
        steelRimmedCrateShaderProgram.setVec3("flashlight.color", flashlightColor); //RED flashlight
        steelRimmedCrateShaderProgram.setVec3("flashlight.position", camera.Position);
        steelRimmedCrateShaderProgram.setVec3("flashlight.direction", camera.Front);
        steelRimmedCrateShaderProgram.setFloat("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
            //flashlight attenuation
        steelRimmedCrateShaderProgram.setFloat("flashlight.constant", 1.0f);
        steelRimmedCrateShaderProgram.setFloat("flashlight.linear", 0.09f);
        steelRimmedCrateShaderProgram.setFloat("flashlight.quadratic", 0.032f);
        // steelRimmedCrateShaderProgram.setVec3("light.direction", sunDirection);
        steelRimmedCrateShaderProgram.setVec3("viewPos", camera.Position);


        // untextured box
        untexturedCubeShaderProgram.use();
        glm::vec3 untexturedMaterialColor = glm::vec3(0.4f, 0.7f, 0.2);
        glm::vec3 diffuseMaterialLevel = glm::vec3(1.0f, 0.5f, 0.31f);
        untexturedCubeShaderProgram.setVec3("material.ambientLevel", ambientMaterialLevel);
        untexturedCubeShaderProgram.setVec3("material.diffuseLevel", diffuseMaterialLevel);
        untexturedCubeShaderProgram.setVec3("material.specularLevel", specularMaterialLevel);
        untexturedCubeShaderProgram.setVec3("material.color", untexturedMaterialColor);
        untexturedCubeShaderProgram.setFloat("material.shininessLevel", shininessMaterialLevel);
        untexturedCubeShaderProgram.setVec3("light.color", lightColor);
        untexturedCubeShaderProgram.setVec3("light.direction", sunDirection);
        untexturedCubeShaderProgram.setVec3("viewPos", camera.Position);
        untexturedCubeShaderProgram.setVec3("light.ambientLevel", ambientLightLevel);
        untexturedCubeShaderProgram.setVec3("light.diffuseLevel", diffuseLightLevel);
        untexturedCubeShaderProgram.setVec3("light.specularLevel", specularLightLevel);

        // create tansformations - instantiate matrices. Reset for each cube
        // otherwise transform matrices accumulate;
        glm::mat4 model_transform;
        glm::mat4 view_transform = camera.GetViewMatrix();
        // // draw normal objects (not light box)
        for(GLuint i = 0; i < 10; i++) {
            model_transform = glm::mat4(1.0f);
            model_transform = glm::translate(model_transform, cubePositions[i]);
            model_transform =
                glm::rotate(model_transform, (float)(SDL_GetTicks64() / 1000.0 + i * 2), glm::vec3(1.0f, 1.0f, 1.0f));
            if(i % 3 == 0) {
                steelRimmedCrateShaderProgram.use();
                steelRimmedCrateShaderProgram.setMat4("model_transform", model_transform);
                steelRimmedCrateShaderProgram.setMat4("view_transform", view_transform);
                steelRimmedCrateShaderProgram.setMat4("projection_transform", projection_transform);
            } else if(i % 2 == 0) {
                woodenBoxShaderProgram.use();
                woodenBoxShaderProgram.setMat4("model_transform", model_transform);
                woodenBoxShaderProgram.setMat4("view_transform", view_transform);
                woodenBoxShaderProgram.setMat4("projection_transform", projection_transform);
            } else {
                untexturedCubeShaderProgram.use();
                untexturedCubeShaderProgram.setMat4("model_transform", model_transform);
                untexturedCubeShaderProgram.setMat4("view_transform", view_transform);
                untexturedCubeShaderProgram.setMat4("projection_transform", projection_transform);
            }
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
