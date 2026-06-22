#ifndef MESH_H
#define MESH_H
#include "Shader.h"

#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

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
    std::vector<GLuint> indices; // TODO: set to uint_32_t?
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    ~Mesh() = default;
    void Draw(Shader& shader);

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    void setupMesh();
};
#endif // MESH_H
