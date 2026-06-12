#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>
#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture {
    GLuint id;
    std::string type;
};
class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    void setupMesh();
};
#endif // MESH_H
