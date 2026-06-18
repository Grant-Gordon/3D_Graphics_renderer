#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

#include "GameObject.h"
#include "LightTypes.h"

#include <vector>

class LevelMap {
public:
    LevelMap();
    LevelMap(std::vector<GameObject> staticObjects, std::vector<PointLight> pointLights, std::vector<SpotLight> spotLights, DirectionalLight sun);
    ~LevelMap() = default;

    std::vector<GameObject> staticObjects;

    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
    DirectionalLight sun;

private:
};
#endif // LEVEL_MAP_H
