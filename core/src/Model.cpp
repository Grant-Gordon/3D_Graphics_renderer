#include "Model.h"

#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <fstream>
#include <glad/glad.h>
#include <iostream>

Model::Model(const std::string& path){
    Model model = deserialize(path);
    for(Mesh mesh : model.meshes){
        for(Texture tex : mesh.textures){
            tex.id = textureFromFile(tex.type, tex.path);
        }
        mesh.setupMesh();
    }


}

static Model Model::import(const char* path) { // assimp converter
    loadModel(path);
}
// clang-format off
    // Model:
    //     std::vector<Mesh> m_meshes;
    //     std::string m_directory;
    //     std::vector<Texture> m_texturesLoaded; // saves from reloading same texture for each mesh
    // Mesh:
    //     struct Vertex {
    //         glm::vec3 position;
    //         glm::vec3 normal;
    //         glm::vec2 texCoords;
    //     };
    //     struct Texture {
    //         GLuint id;
    //         std::string type;
    //         std::string path;
    //     };
    //     std::vector<Vertex> vertices;
    //     std::vector<GLuint> indices;
    //     std::vector<Texture> textures;
// clang-format on

// clang-format off
// MODEL:
//     fileType = MODL_FILE_ID
//     version = MODL_FILE_VERSION
//     meshCount
//     directoryLength
//     directoryRaw
//
//     MESHES[]{
//     ---------------------
//     header{
//     vertexCount
//     indicesCount
//     textureCount
//     }
//
//     VertexDataRAW
//     IndicesDataRAW
//
//     TEXTURES[]{
//     typeLength
//     typeRaw
//     pathLength
//     pathRaw
//     }
//     }
// clang-format on
void Model::serialize(const std::string& outFilename) {
    std::ofstream file(outFilename, ios::binary);
    if(!file.is_open()) {
        std::cerr << "Error::Failed to open file for writing." << std::endl;
        return;
    }

    ModelBinaryHeader modelHeader;
    modelHeader.fileType = MODL_FILE_ID;
    modelHeader.version = MODL_FILE_VERSION;
    modelHeader.meshCount = m_meshes.size();
    file.write(reinterpret_cast<const char*>(&modelHeader), sizeof(ModelBinaryHeader));
    writeString(file, m_directory);

    for(const Mesh& mesh: m_meshes) {
        // Mesh Header
        MeshBinaryHeader meshHeader;
        MeshBinaryHeader meshHeader.vertexCount = mesh.vertices.size();
        MeshBinaryHeader meshHeader.indicesCount = mesh.indices.size();
        MeshBinaryHeader meshHeader.textureCount = mesh.textures.size();
        file.write(reinterpret_cast<const char*>(&meshHeader), sizeof(MeshBinaryHeader));

        // write data
        file.write(reinterpret_cast<const char*>(mesh.vertices.data()),
            sizeof(Vertex) * meshHeader.vertexCount); // should be safe since Vertex is POD (3 glm::vec3's)
        file.write(reinterpret_cast<const char*>(mesh.indices.data()), sizeof(GLuint) * meshHeader.indicesCount);

        for(const Texture& tex: mesh.textures) {
            writeString(file, tex.type);
            writeString(file, tex.path);
        }
    }

    file.close();
    std::cout << "Object serialized successfully." << std::endl;
}

namespace {
    void writeString(std::ofstream& file, const std::string& str) {
        const uint32_t len = str.size();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(str.data(), len);
    }
} // namespace


static Model Model::deserialize(const std::string& filename) {
    Model model{};
    ModelBinaryHeader modelHeader;

    std::ifstream file(filename, ios::binary);
    if(!file.is_open()) {
        std::cerr << "Error:: Failed to open file for reading." << std::endl;
        return model;
    }
    // Note: Read consumes the stream, reads first n
    file.read(reinterpret_cast<char*>(&modelHeader), sizeof(ModelBinaryHeader));
    if(modelHeader.fileType != MODL_FILE_ID) { // TODO: str or uint? need to reinterpt?
        std::cout << "Error headerFileType != " << MODL_FILE_ID
                  << ", modelHeader.fileType = " << std::to_string(modelHeader.filetype) << std::endl;
        return model;
    }
    if(modelHeader.version != MODL_FILE_VERSION) { // TODO: str or uint? need to
                                                                                      // reinterpt?
        std::cout << "Error Model File Version != " << MODL_FILE_VERSION
                  << ", modelHeader.fileType = " << std::to_string(modelHeader.filetype) << std::endl;
        return model;
    }

    model.m_directory = readString(file);


    for(int m{0}; m < modelHeader.meshCount; ++m){
        Mesh mesh{};
        MeshBinaryHeader meshHeader;
        file.read(reinterpret_cast<char*>(&meshHeader), sizeof(MeshBinaryHeader));

        mesh.vertices.resize(meshHeader.vertexCount);
        file.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(Vertex) * meshHeader.vertexCount);

        mesh.indices.resize(meshHeader.indicesCount);
        file.read(reinterpret_cast<char*> (mesh.indices.data()), sizeof(GLuint) * meshHeader.indicesCount);

        for(int t{0}; t < meshHeader.textureCount; t++) {
            Texture tex;
            tex.type = readString(file);
            tex.path = readString(file);
        }
        model.push_back(mesh);
        
    }

    file.close();
    std::cout << "Object deserialized succsessfully." << std::endl;
    return model;
}

namespace {
    std::string readString(ifstream& file) {
        uint32_t len;
        file.read(reinperpret_cast<char*>(&len), sizeof(len));
        std::string str(len, '\0');

        file.read(str.data(), len);
        return str;
    }
} // namespace
// clang-format off
// MODEL:
//     fileType = MODL_FILE_ID
//     version = MODL_FILE_VERSION
//     meshCount
//     directoryLength
//     directoryRaw
//
//     MESHES[]{
//     ---------------------
//     header{
//     vertexCount
//     indicesCount
//     textureCount
//     }
//
//     VertexDataRAW
//     IndicesDataRAW
//
//     TEXTURES[]{
//     typeLength
//     typeRaw
//     pathLength
//     pathRaw
//     }
//     }
// clang-format on
void Model::Draw(Shader& shader) {
    for(size_t i{0}; i < this->m_meshes.size(); ++i) { this->m_meshes[i].Draw(shader); }
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

    this->m_directory = path.substr(0, path.find_last_of('/'));
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
        vertex.position = tempVec3;
        // Normals
        tempVec3.x = mesh->mNormals[i].x;
        tempVec3.y = mesh->mNormals[i].y;
        tempVec3.z = mesh->mNormals[i].z;
        vertex.normal = tempVec3;
        // textures
        if(mesh->mTextureCoords[0]) { // mesh contains texcoords?
            glm::vec2 tempVec2;
            tempVec2.x = mesh->mTextureCoords[0][i].x;
            tempVec2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tempVec2;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
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
            if(std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

GLuint Model::textureFromFile(char const* path, const std::string& directory) {
    std::string filename = directory + '/' + std::string(path);
    GLuint textureID;
    glGenTextures(1, &textureID);

    GLint width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if(!data) {
        std::cout << "Failed to load texture at path: " << filename << std::endl;
        stbi_image_free(data);
        return textureID;
    }
    // specify data format
    GLenum format;
    if(nrComponents == 1) {
        format = GL_RED;
    } else if(nrComponents == 3) {
        format = GL_RGB;
    } else if(nrComponents == 4) {
        format = GL_RGBA;
    } else {
        std::cout << "value of nrComponents (" << nrComponents << ") is not supported for textures" << std::endl;
        stbi_image_free(data);
        return textureID;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // set the texture wrapping/filtering options (on the currently bound texture
    // object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return textureID;
}
