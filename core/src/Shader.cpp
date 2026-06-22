#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file buffer content into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convrt stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: In\n\t " << vertexPath << "\nor\n\t" << fragmentPath
                  << "\n"
                  << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // compile shaders
    GLuint vertex, fragment;
    // v shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");


    // f shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete shaders since linked and no linger neccessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::use() {
    glUseProgram(this->ID);
}

// Utility Unifrom funcs
//-------------------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (GLint)value);
}
void Shader::setInt(const std::string& name, const GLint value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, const GLfloat value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
//-------------------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const GLfloat x, const GLfloat y) const {
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(
    const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat a) const {
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, a);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}
//-------------------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
//-------------------------------------------------------------------------------------


void Shader::checkCompileErrors(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
        } else {

            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
            }
        }
    }
    std::cout << "SHADER_COMPILATION::SUCCESS" << std::endl;
}
