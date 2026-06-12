#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include "Shader.h"

#include <glad/glad.h> //TODO: header gaurds
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model(char* path);
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> texturesLoaded; //saves from reloading same texture for each mesh
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H
