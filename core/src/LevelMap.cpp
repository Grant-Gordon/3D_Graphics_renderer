#include "LevelMap.h"
#include <fstream>

#include "GameObject.h"
#include "LightTypes.h"
#include "Model.h"
#include "json.hpp" //nlohmann::json

#include <string>
#include <vector>

LevelMap::LevelMap(std::string levelPath) {
    std::ifstream file(levelPath);
    nlohmann::json levelJson;
    file >> levelJson;


    // Parse Game Objects
    for(int gameObject: levelJson["gameObjects"]) {
        std::string modelPath = gameObject["model"];
        glm::vec3 position = parseVec3(gameObject["position"]);
        glm::vec3 scale = parseVec3(gameObject["scale"]);
        glm::vec3 rotation = parseVec3(gameObject["rotation"]);



        m_staticObjects.push_back(GameObject{modelPath, Transform{position, rotation, scale}});
    }


    for(int pointLight: levelJson["pointLights"]) {
        m_PointLights.push_back(PointLight{
            parseVec3(pointLight["position"]),
            parseVec3(pointLight["color"]),

            parseVec3(pointLight["ambient"]),
            parseVec3(pointLight["diffuse"]),
            parseVec3(pointLight["specular"]),
            // attenuation
            pointLight["constant"],
            pointLight["linear"],
            pointLight["quadratic"],
        });
    }

    for(int spotLight: levelJson["spotLights"]) {
        m_pointLights.push_back(SpotLight{
            parseVec3(spotLight["position"]),
            parseVec3(spotLight["color"]),
            // phong
            parseVec3(spotLight["ambient"]),
            parseVec3(spotLight["diffuse"]),
            parseVec3(spotLight["specular"]),
            // attenuation
            spotLight["constant"],
            spotLight["linear"],
            spotLight["quadratic"],
            // Cone
            parseVec3(spotLight["direction"]),
            spotlight["outerCutOff"],
            spotlight["innerCutOff"],
        });
    }

    int sun = levelJson["DirectionalLight"];
    m_sun = DirectionalLight{
        parseVec3(sun["direction"],
        parseVec3(sun["color"],
        //phong
        parseVec3(sun["ambient"],
        parseVec3(sun["diffuse"],
        parseVec3(sun["specular"],
    };
}

glm::vec3 LevelMap::parseVec3(const nlohmann::json& arr) {
    return glm::vec3(arr[0], arr[1], arr[2]);
}
