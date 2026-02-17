//system
#include <iostream>

//Window(SDL), graphic spec (openGL) and gpu-driver/spec binder (glad)
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <chrono>
#include "stb_image.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;
std::string TEXTURE_DIR = "/home/emergentstupidity/persProj/3D_Graphics_renderer/sandbox/texture_demo/Textures/";
std::string TEXTURE_FILE = "container.jpg";
std::string TEXTURE_PATH = TEXTURE_DIR + TEXTURE_FILE;

//Vertex shader source code

//Vertex shader out, and fragment shader in must have same name
const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    in vec3 vInPos;
    in vec2 vInTexCoord;

    out vec2 vOutTexCoord; 

    uniform mat4 model_transform;
    uniform mat4 view_transform;
    uniform mat4 projection_transform;
    
    void main(){
        gl_Position = projection_transform * view_transform * model_transform * vec4(vInPos, 1.0f);
        vOutTexCoord=vInTexCoord;
    }
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec2 vOutTexCoord; 

    out vec4 fOutColor;
    uniform sampler2D samplerTexture;
    //unform sampler2D my other texture or whatever. 

    void main(){
        fOutColor = texture(samplerTexture, vOutTexCoord) * vec4(1.0, 1.0, 1.0 , 1.0);
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
    glEnable(GL_DEPTH_TEST); 
    
    
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
    glBindFragDataLocation(shaderProgram, 0, "fOutColor");

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
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    //for multiple cubes:

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    //VAO Vertex Array Object, 
    //VAO stores one or more VBO pointers. Tells GL how to interpret them 
    GLuint VAO, VBO;// EBO;
    //VAO createion must come before VBO
    glGenVertexArrays(1, &VAO);

    //buffers are what is being batched CPU->GPU for reduced IO
    glGenBuffers(1, &VBO);
    //    glGenBuffers(1, &EBO);

    //Make VAO the currect VAO by binding it
    glBindVertexArray(VAO);
    
    //bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //copy vertex data into buffers memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC_DRAW=used many times, STREAM_DRAW = used a few, DYNAMIC DRAW= shanged a lot and used a lot
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //specify layout of vertex data
    //names need to match glsl shader vars
    //position attribute
    GLint posAttrib = glGetAttribLocation(shaderProgram, "vInPos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),(void*)0);
    //color attribute
   // GLint colAttrib = glGetAttribLocation(shaderProgram, "vInColor");
    //glEnableVertexAttribArray(colAttrib);
   // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3* sizeof(GL_FLOAT)));
    //texture coord attribute
    GLuint texAttrib = glGetAttribLocation(shaderProgram, "vInTexCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //Binding has to come AFTER glVertexAttribPointer (yet learnOpenGL doesn't)
    //bind VBO and VAO to 0 to prevent them from being modified;;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);

    //========================
    //load and generate texture 
    //========================

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(TEXTURE_PATH.c_str(), &width, &height, &nrChannels, 0);

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
        
    
    //===========================================
    // Game Loop
    //===========================================
    bool windowShouldClose = false;
    SDL_Event event;

  //  auto t_start = std::chrono::high_resolution_clock::now();

    while (!windowShouldClose) {
        //user input
        while (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                windowShouldClose = true;
            }
        }

        //sets what the background color will clear to when glClear() is called
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //bind texture
        glBindTexture(GL_TEXTURE_2D, texture);
        //if using multiple textures, glActivateTexture(GLTEXTURE<1-16>) the bind, for each texture
        
        // create tansformations - instantiate matrices
        glm::mat4 model_transform = glm::mat4(1.0f);
        glm::mat4 view_transform = glm::mat4(1.0f);
        glm::mat4 projection_transform = glm::mat4(1.0f);
        //set mat vals
       // model_transform = glm::translate(model_transform,  glm::vec3(0.5f, -0.5f, 0.0f));        
        //model_transform = glm::rotate(model_transform,(float)(SDL_GetTicks64()/1000.0), glm::vec3(0.0f, 0.0f, 1.0f));
        //retrieve unform locations
        GLuint modelLoc(glGetUniformLocation(shaderProgram, "model_transform"));
        GLuint viewLoc(glGetUniformLocation(shaderProgram, "view_transform"));
        GLuint projectionLoc(glGetUniformLocation(shaderProgram, "projection_transform"));
        //pass values to the shaders
        


        glUseProgram(shaderProgram);

        //bind VAO so gl knows to use it
        glBindVertexArray(VAO);

        //Draw Triangle using gl primitives
        for(GLuint i=0; i < 10; i++){
            model_transform = glm::rotate(model_transform, (float)(SDL_GetTicks64()/1000.0), glm::vec3(0.5f, 1.0f, 0.0f));
            model_transform = glm::translate(model_transform, cubePositions[i]);

            view_transform= glm::translate(view_transform, glm::vec3(0.0f, 0.0f,-23.0f));
            projection_transform = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_transform));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_transform));
            //Often good pracrice to set outside of the main loop as projection matrix rarely changes
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_transform));
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
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