//system
#include <iostream>

//Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>


int SCREEN_WIDTH = 256;
int SCREEN_HEIGHT = 256;

//Vertex shader source code
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main() {

    //===========================================
    // SDL init
    //===========================================
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_Window* SDL_window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
    if (SDL_window == NULL){
        printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    //create GL Context
    SDL_GLContext glContext = SDL_GL_CreateContext(SDL_window); 
    if ( glContext == NULL){
        printf("OpenGL context could not be created. SDL Error: %s\n", SDL_GetError() );
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    
    //glad: laod all OpenGL function pointers
    if (!gladLoadGLLoader((SDL_GL_GetProcAddress))){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    
    //vertex shader doing the actual vertex position transformations. 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach shdr src to vertex shader obj 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compile Shader to machine code
    glCompileShader(vertexShader);
    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    }else{
        std::cout << "SUCCESS::SHADER::VERTEX:COMPILATION" << std::endl;
    }

    //fragment shader colors pixels in yo polygon 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach frag shdr src to obj 
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compule shader to Machine code 
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
    }else{
        std::cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION" << std::endl;
    }
    //Create Shader program obj
    GLuint shaderProgram = glCreateProgram();
    // Attach shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Wrap-up/Link all shaders into shader program
    glLinkProgram(shaderProgram); 

    //test program compilation
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
        std::cout << "ERROR::SHADERPROGRAM::LINK_FAILURE\n" << infolog << std::endl;
    }else{
        std::cout << "SUCCESS::SHADERPROGRAM::LINK\n" << std::endl;
    }
    glUseProgram(shaderProgram);
    // Can delete shader objs (now that in program?)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
        
    //vertex pos 
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.0f, 0.0f,
    };






    //VAO Vertex Array Object, 
    //VAO stores one or more VBO pointers. Tells GL how to interpret them 

    //VBO
    //VAO Vertex Array Object, 

    GLuint VAO, VBO;
    //VAO createion must come before VBO
    glGenVertexArrays(1, &VAO);

    //buffers are what is being batched CPU->GPU for reduced IO
    glGenBuffers(1, &VBO);

    //Make VAO the currect VAO by binding it
    glBindVertexArray(VAO);
    //bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //copy vertex data into buffers memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC_DRAW=used many times, STREAM_DRAW = used a few, DYNAMIC DRAW= shanged a lot and used a lot

    //Confif v attr so gl knows how to read VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable v attr so gl knows to use it
    glEnableVertexAttribArray(0);


    //bind VBO and VAO to 0 to prevent them from being modified
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    
    //===========================================
    // Game Loop
    //===========================================
    bool windowShouldClose = false;
    SDL_Event event;
    
    while (!windowShouldClose) {
        while (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                windowShouldClose = true;
            }
        }
        
        //set background color / clear screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        
        
        //tell gl which shader program to use
        glUseProgram(shaderProgram);

        //bind VAO so gl knows to use it
        glBindVertexArray(VAO);

        //Draw Triangle using gl primitives
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(SDL_window);        
    }

    //===========================================
    // Deconstructors/ End Game Loop    
    //===========================================
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    SDL_DestroyWindow(SDL_window);
    return 0;
}