#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <memory>
#include "Model.h"
#include <unordered_map>
#include <string>

class AssetManager{

public:
    std::shared_ptr<Model> loadModel(const std::string& path);

private:
    std::unordered_map<std::string, std::shared_ptr<Model>> m_modelCache; //(name, model*)

};
#endif // ASSET_MANAGER_H
