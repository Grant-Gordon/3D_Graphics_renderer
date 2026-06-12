#include "Model.h"

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <iostream>


Model::Model(char* path) {
    loadModel(path);
}

void Model::Draw(Shader& shader) {
    for(size_t i{0}; i < this->meshes.size(); ++i) { this->meshes[i].Draw(shader); }
}

void Model::loadModel(std::string path) {

    Assimp::Importer importer;
    // aiProcess_Tirangulate = set all primitives to triangles
    // useful flags, aiProcess_{GenNormals, SplitLargeMeshes, OptimizeMeshes}
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes
    for(size_t i{0}; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // process all children
    for(size_t i{0}; i < node->mNumChildren; ++i) { processNode(node->mChildren[i], scene); }
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // process vertex pos, norm, and tex coords
    for(size_t i{0}; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 tempVec3; // assimp uses own vec types so convert to glm
        // vertes position
        tempVec3.x = mesh->mVertices[i].x;
        tempVec3.y = mesh->mVertices[i].y;
        tempVec3.z = mesh->mVertices[i].z;
        vertex.Position = tempVec3;
        // Normals
        tempVec3.x = mesh->mNormals[i].x;
        tempVec3.y = mesh->mNormals[i].y;
        tempVec3.z = mesh->mNormals[i].z;
        vertex.Normal = tempVec3;
        // textures
        if(mesh->mTextureCoords[0]) { // mesh contains texcoords?
            glm::vec2 tempVec2;
            tempVec2.x = mesh->mTextureCoords[0][i].x;
            tempVec2.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = tempVec2;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }


    // process indices
    for(size_t i{0}; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(size_t j{0}; j < face.mNumIndices; ++j) { indices.push_back(face.mIndices[j]); }
    }
    ////process materials
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

    std::vector<Texture> textures;
    for(size_t i{0}; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(size_t j{0}; j < texturesLoaded.size(); ++j) {
            if(std::strcmp(texturesLoaded[j].path.data(), str.C_str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_str(), directory);
            texture.type = typeName;
            texture.path = str.C_str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}
