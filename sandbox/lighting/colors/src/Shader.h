#ifndef SHADER_H
#define SHADER_H
#include <string>

class shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();


    // Utility Unifrom funcs
    void Shader::setBool(const std::&name, bool value) const;


    void Shader::setInt(const std::&name, int value) const;
    void Shader::setFloat(const std::&name, float value) const;


private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif // SHADER_H
