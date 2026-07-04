#include "LevelMap.h"

#include "GameObject.h"
#include "LightTypes.h"
#include "Model.h"
#include "json.hpp" //nlohmann::json

#include <fstream>
#include <string>
#include <vector>


glm::vec3 LevelMap::parseVec3(const nlohmann::json& arr) {
    return glm::vec3(arr[0], arr[1], arr[2]);
}
Transform LevelMap::parseTransform(const nlohmann::json& json) {
    return Transform{
        parseVec3(json["position"]),
        parseVec3(json["scale"]),
        parseVec3(json["rotation"]),
    };
}
Asset LevelMap::parseAssetModel(const nlohmann::json& json) {
    return Asset{
        json[0],
        json[1],
    };
}
PhongProperties LevelMap::parsePhong(const nlohmann::json& json) {
    return PhongProperties{
        parseVec3(json["ambient"]),
        parseVec3(json["diffuse"]),
        parseVec3(json["specular"]),
    };
}
Attenuation LevelMap::parseAttenuation(const nlohmann::json& json) {
    return Attenuation{
        json["constant"],
        json["linear"],
        json["quadratic"],
    };
}
GameObject LevelMap::parseGameObject(const nlohmann::json& json, AssetManager& assetManager) {
    return GameObject{
        assetManager.getModel(parseAssetModel(json["assetModel"])),
        parseTransform(json),
    };
}
PointLight LevelMap::parsePointLigth(const nlohmann::json& json, AssetManager& assetManager) {
    return PointLight{
        parseGameObject(json, assetManager),
        parseVec3(json["position"]),
        parseVec3(json["color"]),

        parsePhong(json),
        parseAttenuation(json),
    };
}
SpotLight LevelMap::parseSpotLight(const nlohmann::json& json, AssetManager& assetManager) {
    // clang-format off
    return SpotLight{
        parseGameObject(json, assetManager);
        parseVec3(json["position"]), 
        parseVec3(json["color"]), 

        parsePhong(json), 
        parseAttenuation(json),

        parseVec3(json["direction"]), 
        json["outCutOff"],
        json["innderCutOff"],
    };
    // clang-format on
}

DirectionalLight LevelMap::parseDirectionalLight(const nlohmann::json& json) {
    return DirectionalLight{
        parseVec3(json["direction"]),
        parseVec3(json["color"]) parsePhong(json),
    };
}

LevelMap::LevelMap(const std::string& levelPath, AssetManager& assetManager) {
    std::ifstream file(levelPath);
    nlohmann::json levelJson;
    file >> levelJson;

    // Parse Game Objects
    for(const nlohmann::json& gameObject: levelJson["gameObjects"]) {
        m_staticObjects.push_back(parseGameObject(gameObject, assetManager);
    }
    // Point Lights
    for(const nlohmann::json& pointLight: levelJson["pointLights"]) {
        m_pointLights.push_back(parsePointLight(pointLight, assetManager);
    }
    // SpotLights
    for(const nlohmann::json& spotLight: levelJson["spotLights"]) {
        m_spotLights.push_back(parseSpotLight(spotLight, assetManager);
    }
    // Directional Light
    const nlohmann::json& directionalLight = levelJson["DirectionalLight"];
    m_directionalLight = parseDirectionalLight(directionalLight, assetManager);
}
