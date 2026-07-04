#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

#include "AssetManager.h"
#include "GameObject.h"
#include "LightTypes.h"
#include <json.hpp>

#include <string>
#include <vector>

class LevelMap {
public:
    LevelMap();
    // TODO: figure out consts
    LevelMap(std::string levelPath, AssetManager& assetmanager);
    ~LevelMap() = default;

    // Getters
    const std::vector<GameObject> getStaticObjects() const;

    const std::vector<PointLight> getPointLights() const;
    const std::vector<SpotLight> getSpotLights() const;
    const DirectionalLight getDirectionalLight() const;

private:
    std::vector<GameObject> m_staticObjects;

    std::vector<PointLight> m_pointLights;
    std::vector<SpotLight> m_spotLights;
    DirectionalLight m_directionalLight;

    static glm::vec3 parseVec3(const nlohmann::json& arr);
    static Transform parseTransform(const nlohmann::json& json);
    static Asset parseAssetModel(const nlohmann::json& json);
    static PhongProperties parsePhong(const nlohmann::json& json);
    static Attenuation parseAttenuation(const nlohmann::json& json);
    static GameObject parseGameObject(const nlohmann::json& json, AssetManager& assetManager);
    static PointLight parsePointLigth(const nlohmann::json& json, AssetManager& assetManager);
    static SpotLight parseSpotLight(const nlohmann::json& json, AssetManager& assetManager);
    static DirectionalLight parseDirectionalLight(const nlohmann::json& json);
};
#endif // LEVEL_MAP_H
