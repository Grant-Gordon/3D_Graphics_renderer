#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glad/glad.h>
class Shader {
public:
     GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();


    // Utility Unifrom funcs
    void setBool(const std::string &name, bool value) const;


    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;


private:
    void checkCompileErrors(GLuint shader, std::string type);
};
#endif // SHADER_H
