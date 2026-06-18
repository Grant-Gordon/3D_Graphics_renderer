#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>
#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};
struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};
class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    ~Mesh() = default;
    void Draw(Shader &shader);
private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    void setupMesh();
};
#endif // MESH_H
