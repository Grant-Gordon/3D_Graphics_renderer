//system
#include <iostream>

//Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 1024;

//Vertex shader source code
//Vertex shader out, and fragment shader in must have same name
const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec3 aPos;
    in vec3 inColor;
    out vec3 Color; 
    void main(){
        Color=inColor;
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 Color; 
    out vec4 FragColor;
    // uniform vec3 triangleColor;
    void main(){
        FragColor = vec4(Color, 1.0f);
    }


)glsl";

int main() {

    //===========================================
    // SDL init
    //===========================================
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    //TODO: glViewport(GLuint x Gluint y, width, height); // For normalizing I beleive?
    SDL_Window* SDL_window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (SDL_window == NULL){
        printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //create GL Context
    SDL_GLContext glContext = SDL_GL_CreateContext(SDL_window); 
    if ( glContext == NULL){
        printf("OpenGL context could not be created. SDL Error: %s\n", SDL_GetError() );
        return -1;
    }

    
    
    //glad: laod all OpenGL function pointers
    if (!gladLoadGLLoader((SDL_GL_GetProcAddress))){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
   
    glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
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
    glBindFragDataLocation(shaderProgram, 0, "FragColor");

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
        -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
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

    // //Confif v attr so gl knows how to read VBO
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable v attr so gl knows to use it
    // glEnableVertexAttribArray(0);



    //specify layout of vertex data
    //names need to match glsl shader vars
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),0);
    
    GLint colAttrib = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3* sizeof(GL_FLOAT)));

    //Binding has to come AFTER glVertexAttribPointer
    //bind VBO and VAO to 0 to prevent them from being modified
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    //for uniform color   //set color of triangle
//    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor"); 
    
    //===========================================
    // Game Loop
    //===========================================
    bool windowShouldClose = false;
    SDL_Event event;

    auto t_start = std::chrono::high_resolution_clock::now();

    while (!windowShouldClose) {
        while (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                windowShouldClose = true;
            }
        }
        // auto t_now = std::chrono::high_resolution_clock::now();
        // float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        
       // glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        //sets what the background color will clear to when glClear() is called
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
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
