#include "AssetManager.h"

#include "Model.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

void AssetManager::loadModel(const Asset& asset) {
    m_modelCache.insert({asset.name, Model.deserialize(asset.path)});
}

std::shared_ptr<Model> AssetManager::getModel(const Asset& asset) {
    if(!m_modelCache.contains(asset.name){
        std::cerr << "Error: Failed to retrieve model. Model not found in AssetManager.m_modelCache. asset.name: "
                  << asset.name << std::endl;
        return nullptr;
    };
    return m_modelCache[asset.name];
}
