#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() = default;

    void use();

    // Utility Unifrom funcs
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, const GLint value) const;
    void setFloat(const std::string& name, const GLfloat value) const;

    void setVec2(const std::string& name, const GLfloat x, const GLfloat y) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;

    void setVec3(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec4(const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat a) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;


private:
    void checkCompileErrors(GLuint shader, std::string type);
};
#endif // SHADER_H
