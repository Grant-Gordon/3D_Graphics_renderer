#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>


static constexpr uint32_t MODL_FILE_ID = 0x4D4F444C; //"MODL" //notsure the use of static here
static constexpr uint32_t MODL_FILE_VERSION = 1; //"MODL" //notsure the use of static here
struct ModelBinaryHeader {
    uint32_t fileType;
    uint32_t version;
    uint32_t meshCount;

};
struct MeshBinaryHeader {
    uint32_t vertexCount;
    uint32_t indicesCount;
    uint32_t textureCount;
};


class Model {
public:
    Model(const std::string& path);
    static Model import(const char* path); //assimp converter
    static Model deserialize(const std::string& filename); //constructor from model binary

    void serialize(const std::string& outFilename);
    void Draw(Shader& shader);
    
    ~Model() = default;

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_texturesLoaded; // saves from reloading same texture for each mesh

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    GLuint textureFromFile(char const* path, const std::string& directory);
};

#endif // MODEL_H
