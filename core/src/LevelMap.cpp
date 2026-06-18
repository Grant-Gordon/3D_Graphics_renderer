#include "LevelMap.h"

#include "LightTypes.h"

LevelMap::LevelMap();

LevelMap::LevelMap(std::vector<GameObject> staticObjects,
    std::vector<PointLight> pointLights,
    std::vector<SpotLight> spotLights,
    DirectionalLight sun):
    staticObjects(staticObjects),
    pointLights(pointLights),
    spotLights(spotLights),
    sun(sun) {};
