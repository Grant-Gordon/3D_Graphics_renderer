// system
#include <iostream>

// Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <SDL.h>
#include <SDL_image.h>
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
std::string VERTEX_SHADER_PATH = SHADERS_DIR + "3.3.shader.vs";
std::string FRAGMENT_SHADER_PATH = SHADERS_DIR + "3.3.shader.fs";

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
    // TODO: idk where this line camerfrom or how to fit it into the shader abstraction
    //    glBindFragDataLocation(shaderProgram, 0, "fOutColor");


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

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, -15.0f),
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
    // VAO Vertex Array Object,
    // VAO stores one or more VBO pointers. Tells GL how to interpret them
    GLuint VAO, VBO; // EBO;
    // VAO createion must come before VBO
    glGenVertexArrays(1, &VAO);

    // buffers are what is being batched CPU->GPU for reduced IO
    glGenBuffers(1, &VBO);
    //    glGenBuffers(1, &EBO);

    // Make VAO the currect VAO by binding it
    glBindVertexArray(VAO);

    // bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // copy vertex data into buffers memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // STATIC_DRAW=used many times, STREAM_DRAW = used a few,
    // DYNAMIC DRAW= shanged a lot and used a lot
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    // GL_STATIC_DRAW);

    // specify layout of vertex data
    // names need to match glsl shader vars
    // position attribute
    // TODO: doesn't match opengl's basic attribPonter + EnableVertexAttribArray
    GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "vInPos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    // color attribute
    // GLint colAttrib = glGetAttribLocation(shaderProgram, "vInColor");
    // glEnableVertexAttribArray(colAttrib);
    // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 *
    // sizeof(GLfloat), (void*)(3* sizeof(GL_FLOAT)));
    // texture coord attribute
    GLuint texAttrib = glGetAttribLocation(shaderProgram.ID, "vInTexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Binding has to come AFTER glVertexAttribPointer (yet learnOpenGL doesn't)
    // bind VBO and VAO to 0 to prevent them from being modified;;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
    shaderProgram.setInt("texture", 0);
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
    while(!windowShouldClose) {
        int startFrameTicks = SDL_GetTicks();
        // user input
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    windowShouldClose = true;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_h:
                            // left
                            cameraDirection = Camera::CameraMovement::LEFT;
                            break;
                        case SDLK_j:
                            // backward
                            cameraDirection = Camera::CameraMovement::BACKWARD;
                            break;
                        case SDLK_k:
                            // forward
                            cameraDirection = Camera::CameraMovement::FORWARD;
                            break;
                        case SDLK_l:
                            // right-
                            cameraDirection = Camera::CameraMovement::RIGHT;
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

        GLuint modelLoc(glGetUniformLocation(shaderProgram.ID, "model_transform"));
        GLuint viewLoc(glGetUniformLocation(shaderProgram.ID, "view_transform"));
        GLuint projectionLoc(glGetUniformLocation(shaderProgram.ID, "projection_transform"));
        // pass values to the shaders
        shaderProgram.use();

        // bind VAO so gl knows to use it
        glBindVertexArray(VAO);

        // Draw Triangle using gl primitives
        for(GLuint i = 0; i < 10; i++) {
            // create tansformations - instantiate matrices. Reset for each cube
            // otherwise transform matrices accumulate;
            glm::mat4 model_transform = glm::mat4(1.0f);
            glm::mat4 view_transform = glm::mat4(1.0f);
            glm::mat4 projection_transform = glm::mat4(1.0f);

            model_transform = glm::translate(model_transform, cubePositions[i]);
            model_transform =
                glm::rotate(model_transform, (float)(SDL_GetTicks64() / 1000.0 + i * 2), glm::vec3(1.0f, 1.0f, 1.0f));
            view_transform = camera.GetViewMatrix();
            // view_transform= glm::translate(view_transform, glm::vec3(0.0f,
            // 0.0f,-05.0f));
            projection_transform =
                glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 100.0f);
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f,
            // 0.5f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_transform));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_transform));
            // Often good pracrice to set outside of the main loop as projection
            // matrix rarely changes
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_transform));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
