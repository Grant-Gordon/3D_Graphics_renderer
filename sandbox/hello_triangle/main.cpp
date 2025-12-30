//system
#include <iostream>

//Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>
//My files
#include "RenderWindow.h"


int SCREEN_WIDTH = 256;
int SCREEN_HEIGHT = 256;

int main(int argc, char* args[]) {
    //SDL init
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    
    //Create SDL Window and Renderer
    RenderWindow window("SDL Window", SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = window.GetRenderer();
   
    if (renderer == NULL){
        printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
        return -1
    }

    //create GL Context
    SDL_GLContext gContext = SDL_GL_CreateContext(window); //TODO: passing in reference to RenderWindow not SDL windo
    if ( gContext == NULL){
        printf(" OpenGL context could not be created. SDL Error: %s\n", SDL_GetError() );
        return -1;
    }



    //glad: laod all OpenGL function pointers
    if (!gladLoadGLLoader((SDL_GL_GetProcAddress))){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Main Render loop
    bool windowShouldClose = false;
    SDL_Event event;

    while (!windowShouldClose) {
        while (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }

        //set Renderer colo
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 1);

        //Clear screen
        SDL_RenderClear(renderer);

        //update screen
        SDL_RenderPresent(renderer);

    }

    window.close();

    return 0;
}


//openGL stuff

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, -0.0f, 0.0f,
}


//vertex buffer object
unsigned int VBO;
glGenBuffers(1, &VBO);

//bind buffers
glBindBuffers(GL_ARRAY_BUFFER, VBO);

//copy vertex data into buffers memory
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC_DRAW=used many times, STREAM_DRAW = used a few, DYNAMIC DRAW= shanged a lot and used a lot



//example sourcecode of a GLSL shader (GL shader langauge)
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);




    //fragment shader src e.g:
// version 330 core
// out vec4 FragColor;

// void main()
// {
//     FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
// }

unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);


unsigned int shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram); 


glUseProgram(shaderProgram);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
