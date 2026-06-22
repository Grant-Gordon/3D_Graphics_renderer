#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "Model.h"

#include <memory>
#include <string>
#include <unordered_map>

struct Asset{
    std::string name;
    std::string path;
};

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager() = default;

    void loadModel(const Asset& asset);
    std::shared_ptr<Model> getModel(const Asset& asset);

private:
    std::unordered_map<std::string, std::shared_ptr<Model>> m_modelCache; //(name, model*)
};
#endif // ASSET_MANAGER_H
