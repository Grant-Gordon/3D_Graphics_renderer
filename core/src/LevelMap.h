#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

#include "GameObject.h"
#include "LightTypes.h"

#include <vector>
#include <string>

class LevelMap {
public:
    LevelMap();
    //TODO: figure out consts
    LevelMap(std::string levelPath);
    ~LevelMap() = default;

    //Getters
    const std::vector<GameObject> getStaticObjects() const;

    const std::vector<PointLight> getPointLights() const;
    const std::vector<SpotLight> getSpotLights() const;
    const DirectionalLight getSun() const;

private:
    std::vector<GameObject> m_staticObjects;

    std::vector<PointLight> m_pointLights;
    std::vector<SpotLight> m_spotLights;
    DirectionalLight m_sun;
};
#endif // LEVEL_MAP_H
