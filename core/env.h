#pragma once

#include <string>

namespace env {
    inline const std::string PROJECT_DIR = "/home/emergentstupidity/persProj/3D_Graphics_renderer/";
    inline const std::string CORE_DIR = PROJECT_DIR + "core/";
    inline const std::string EXTERNALS_DIR = CORE_DIR + "externals/";
    inline const std::string SRC_DIR = CORE_DIR + "src/";

    inline const std::string ASSETS_DIR = CORE_DIR + "Assets/";
    inline const std::string TEXTURES_DIR = ASSETS_DIR + "Textures/";
    inline const std::string RAW_MODELS_DIR = ASSETS_DIR + "Raw_Models/";
    inline const std::string PREPROCCESSED_MODELS_DIR = ASSETS_DIR + "Preproccessed_Models/";

    inline const std::string SCRIPTS_DIR = SRC_DIR + "scripts/";
    inline const std::string LEVELS_DIR = SRC_DIR + "Levels/";
    inline const std::string SHADERS_DIR = SRC_DIR + "Shaders/";
} // namespace env
